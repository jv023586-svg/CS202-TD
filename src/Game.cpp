#include "Game.h"
#include "Enemy.h"
#include "Capsule.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>

Game::Game()
    : window(sf::VideoMode({1920, 1080}), "Game"),
    currentState(GameState::LOADING), // Initial state
    debugText(font),
    introText(font),
    stateTimer(0.0f), // Example timer (useful for intro/loading screens)
    introTextY(0.0f),
    introScrollSpeed(30.0f),
    introFadeTopFraction(0.25f),
    introFadeBottomFraction(0.75f),
    introBackgroundFadeAlpha(255.0f),
    introBackgroundFadeSeconds(1.5f),
    introFadeOverlay(),
    gameOver(false), // Game over flag
    gameWon(false) // Game won flag
{
    window.setFramerateLimit(60); // Sets the frame rate to 60 FPS
    
    if (!font.openFromFile("assets/fonts/Arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }

    debugText.setFont(font);
    debugText.setCharacterSize(32);
    debugText.setFillColor(sf::Color::White);
    debugText.setPosition({100.f, 250.f});

    introText.setFont(font);
    introText.setCharacterSize(28);
    introText.setFillColor(sf::Color::White);
    introText.setPosition({80.f, 0.f});
    introFadeOverlay.setPosition({0.f, 0.f});
    introFadeOverlay.setFillColor(sf::Color::Black);

    loadIntroTextFromFile();
    loadBackgroundTextures();
    resetIntroSequence();
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        processEvents(); // Handles window events, such as closing the window and user input
        update(dt); // Updates the game state, such as the position of the objects
        render(); // Renders the game state to the screen
    }
}

void Game::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else {
            switch (currentState) {
                case GameState::LOADING:    processLoadingEvents(*event); break;
                case GameState::MENU:       processMenuEvents(*event); break;
                case GameState::INTRO:      processIntroEvents(*event); break;
                case GameState::TUTORIAL:   processTutorialEvents(*event); break;
                case GameState::PLAYING:    processPlayingEvents(*event); break;
                case GameState::WIN:        processWinEvents(*event); break;
                case GameState::LOSE:       processLoseEvents(*event); break;
                default: break;
                }
        }
    }
}

void Game::update(float dt) {// Updates the game state, such as the position of the objects
    capsule.update(dt); // Updates the position of the capsule
    
    switch (currentState) {
        case GameState::LOADING:    updateLoading(dt); break;
        case GameState::MENU:       updateMenu(dt); break;
        case GameState::INTRO:      updateIntro(dt); break;
        case GameState::TUTORIAL:   updateTutorial(dt); break;
        case GameState::PLAYING:    updatePlaying(dt); break;
        case GameState::WIN:        updateWin(dt); break;
        case GameState::LOSE:       updateLose(dt); break;
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    drawBackgroundForState(currentState);
    switch (currentState) {
        case GameState::LOADING:    renderLoading(); break;
        case GameState::MENU:       renderMenu(); break;
        case GameState::INTRO:      renderIntro(); break;
        case GameState::TUTORIAL:   renderTutorial(); break;
        case GameState::PLAYING:    renderPlaying(); break;
        case GameState::WIN:        renderWin(); break;
        case GameState::LOSE:       renderLose(); break;
    }
    window.display();
}
// ------- LOADING -------
void Game::updateLoading(float dt) {
    stateTimer += dt;
    if (stateTimer > 5.0f) {
        currentState = GameState::MENU;
        stateTimer = 0.f;
    }
}

void Game::renderLoading() {
    debugText.setString("LOADING...");
    window.draw(debugText);
}

void Game::processLoadingEvents(const sf::Event&) {}

// ------- MENU -------
void Game::processMenuEvents(const sf::Event& event) // Process the menu events
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) // If a key is pressed
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) // If the enter key is pressed
        {
            currentState = GameState::INTRO; // Set the current state to the intro state
            stateTimer = 0.f; // Set the state timer to 0
            resetIntroSequence(); // Reset the intro sequence
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            window.close(); // Close the window if the escape key is pressed
        }
    }
}

void Game::updateMenu(float dt) { // Update the menu
    (void)dt;
}

void Game::renderMenu() { // Render the menu
    debugText.setString("MENU\nPress ENTER to start\nPress ESCAPE to exit"); // Set the string of the debug text to the menu message    
    window.draw(debugText); // Draw the debug text
}

// ------- INTRO -------
void Game::updateIntro(float dt) { // Update the intro
    if (introBackgroundFadeAlpha > 0.0f) {
        const float fadeRate = 255.0f / introBackgroundFadeSeconds; // Calculate the fade rate  
        introBackgroundFadeAlpha = std::max(0.0f, introBackgroundFadeAlpha - (fadeRate * dt)); // Update the alpha of the intro background fade overlay
        return;
    }

    introTextY -= introScrollSpeed * dt; // Update the y position of the intro text
    introText.setPosition({80.f, introTextY}); // Set the position of the intro text to 80, introTextY

    const float screenHeight = static_cast<float>(window.getSize().y); // Get the height of the window
    const float fadeTopY = screenHeight * introFadeTopFraction;
    const float fadeBottomY = screenHeight * introFadeBottomFraction; // Get the bottom y position of the fade
    const float textHeight = introText.getLocalBounds().size.y; // Get the height of the intro text
    const float textCenterY = introTextY + (textHeight * 0.5f); // Get the center y position of the intro text

    float alpha = 255.0f;
    if (textCenterY > fadeBottomY) { // If the center y position of the intro text is greater than the bottom y position of the fade
        const float denom = std::max(1.0f, screenHeight - fadeBottomY);
        alpha = 255.0f * std::clamp((screenHeight - textCenterY) / denom, 0.0f, 1.0f); // Update the alpha of the intro text
    } else if (textCenterY < fadeTopY) {
        const float denom = std::max(1.0f, fadeTopY); // Update the alpha of the intro text
        alpha = 255.0f * std::clamp(textCenterY / denom, 0.0f, 1.0f); // Update the alpha of the intro text
    }

    introText.setFillColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(alpha))); // Set the fill color of the intro text to white    

    if (introTextY + textHeight < 0.0f) { // If the y position of the intro text is less than 0
        currentState = GameState::TUTORIAL; // Set the current state to the tutorial state
        stateTimer = 0.f; // Set the state timer to 0
    }
}

void Game::renderIntro() { // Render the intro
    if (introBackgroundFadeAlpha <= 0.0f) { // If the alpha of the intro background fade overlay is less than or equal to 0     
        window.draw(introText); // Draw the intro text
    }
    introFadeOverlay.setSize(sf::Vector2f(window.getSize())); // Set the size of the intro fade overlay to the size of the window
    introFadeOverlay.setFillColor(sf::Color(
        0,
        0,
        0,
        static_cast<std::uint8_t>(std::clamp(introBackgroundFadeAlpha, 0.0f, 255.0f)) // Update the alpha of the intro fade overlay
    ));
    window.draw(introFadeOverlay); // Draw the intro fade overlay
}

void Game::processIntroEvents(const sf::Event& event) {
    // Process the intro events
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) { // If a key is pressed
        if (keyPressed->scancode == sf::Keyboard::Scancode::Space
            || keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
            currentState = GameState::TUTORIAL; // Set the current state to the tutorial state
            stateTimer = 0.f; // Set the state timer to 0
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            window.close(); // Close the window if the escape key is pressed
        }
    }
}

void Game::resetIntroSequence() {
    // Reset the intro sequence
    introBackgroundFadeAlpha = 255.0f; // Set the alpha of the intro background fade overlay to 255
    introText.setString(introTextContent); // Set the string of the intro text to the intro text content    
    const float screenHeight = static_cast<float>(window.getSize().y); // Get the height of the window
    introTextY = screenHeight + 20.0f; // Set the y position of the intro text to the screen height + 20
    introText.setPosition({80.f, introTextY}); // Set the position of the intro text to 80, introTextY
    introText.setFillColor(sf::Color::White); // Set the fill color of the intro text to white
}

void Game::loadBackgroundTextures() {
    // Paths to the background textures
    static constexpr std::array<const char*, kBackgroundCount> paths = {
        "assets/backgrounds/loading.png",
        "assets/backgrounds/menu.png",
        "assets/backgrounds/intro.png",
        "assets/backgrounds/gameplay.png"
    };

    // Load the background textures
    for (std::size_t i = 0; i < paths.size(); ++i) {
        backgroundLoaded[i] = backgroundTextures[i].loadFromFile(paths[i]);
        if (!backgroundLoaded[i]) {
            std::cerr << "Failed to load background: " << paths[i] << '\n';
        }
    }
}

Game::BackgroundId Game::backgroundForState(GameState state) const {
    // Return the background ID for the given game state
    switch (state) {
        case GameState::LOADING: return BackgroundId::LOADING;
        case GameState::MENU: return BackgroundId::MENU;
        case GameState::INTRO: return BackgroundId::INTRO;
        case GameState::TUTORIAL:
        case GameState::PLAYING:
        case GameState::WIN:
        case GameState::LOSE:
            return BackgroundId::GAMEPLAY;
    }
    return BackgroundId::GAMEPLAY;
}

void Game::drawBackgroundForState(GameState state) {
    // Draw the background for the given game state
    const std::size_t index = static_cast<std::size_t>(backgroundForState(state)); // Get the index of the background texture
    if (index >= backgroundTextures.size() || !backgroundLoaded[index]) { // If the index is out of bounds or the background texture is not loaded, return
        return;
    }

    sf::Sprite backgroundSprite(backgroundTextures[index]); // Create a sprite for the background texture
    const auto textureSize = backgroundTextures[index].getSize(); // Get the size of the background texture
    if (textureSize.x == 0 || textureSize.y == 0) {
        return; // If the size of the background texture is 0, return
    }

    backgroundSprite.setScale({ // Set the scale of the background sprite
        static_cast<float>(window.getSize().x) / static_cast<float>(textureSize.x),
        static_cast<float>(window.getSize().y) / static_cast<float>(textureSize.y)
    });
    window.draw(backgroundSprite); // Draw the background sprite
}

void Game::loadIntroTextFromFile() {
    std::ifstream file("assets/intro/intro.txt"); // Open the intro text file
    if (!file.is_open()) {
        introTextContent = "INTRO TEXT FILE NOT FOUND\n\nCreate assets/intro/intro.txt"; // If the intro text file is not found, set the intro text content to the default message
        return;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    introTextContent = buffer.str();
}

// ------- TUTORIAL -------
void Game::processTutorialEvents(const sf::Event& event) {
    // Process the tutorial events
    if (event.is<sf::Event::KeyPressed>()) { // If a key is pressed
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                currentState = GameState::PLAYING;
                stateTimer = 0.f;
            }
        }
    }
}

void Game::updateTutorial(float dt) { // Update the tutorial
    (void)dt;
}

void Game::renderTutorial() { // Render the tutorial
    
    capsule.draw(window); // Draw the capsule
    debugText.setString("TUTORIAL\nPress SPACE to continue"); // Set the string of the debug text to the tutorial message    
    window.draw(debugText);
}

// ------- PLAYING -------
void Game::processPlayingEvents(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) { // If a key is pressed
        if (keyPressed->scancode == sf::Keyboard::Scancode::W) {
            currentState = GameState::WIN; // Set the current state to the win state
            stateTimer = 0.f; // Set the state timer to 0
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::L) {
            currentState = GameState::LOSE; // Set the current state to the lose state
            stateTimer = 0.f; // Set the state timer to 0
        }
    }
}

void Game::updatePlaying(float dt) { // Update the playing
    // THIS is where your real game logic will go
}

void Game::renderPlaying() { // Render the playing  
   // capsule.setScale({0.1f, 0.1f}); // Set the scale of the capsule sprite to 0.1f
    capsule.draw(window);
    debugText.setString("PLAYING\nPress W = WIN, L = LOSE"); // Set the string of the debug text to the playing message    
    window.draw(debugText); // Draw the debug text
}

// ------- WIN -------
void Game::processWinEvents(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) { // If a key is pressed
        if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
            currentState = GameState::MENU; // Set the current state to the menu state
            stateTimer = 0.f; // Set the state timer to 0
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            window.close(); // Close the window if the escape key is pressed
        }
    }
}

void Game::updateWin(float dt) { // Update the win
    (void)dt;
}

void Game::renderWin() { // Render the win
    debugText.setString("YOU WIN\nPress ENTER for main menu\nPress ESCAPE to quit");
    window.draw(debugText); // Draw the debug text
}

// ------- LOSE -------
void Game::processLoseEvents(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) { // If a key is pressed
        if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
            currentState = GameState::MENU; // Set the current state to the menu state
            stateTimer = 0.f; // Set the state timer to 0   
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            window.close(); // Close the window if the escape key is pressed
        }
    }
}

void Game::updateLose(float dt) { // Update the lose
    (void)dt;
}

void Game::renderLose() { // Render the lose
    debugText.setString("YOU LOSE\nPress ENTER for main menu\nPress ESCAPE to quit");
    window.draw(debugText); // Draw the debug text
}