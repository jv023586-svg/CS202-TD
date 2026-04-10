#include "Game.h"
#include <iostream>
#include <optional>

Game::Game()
    : window(sf::VideoMode({800, 600}), "Game"),
    currentState(GameState::LOADING), // Initial state
    debugText(font), 
    stateTimer(0.0f), // Example timer (useful for intro/loading screens)
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
    capsule.update(); // Updates the position of the capsule
    
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
    window.clear();
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
    if (stateTimer > 1.0f) {
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
void Game::processMenuEvents(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
        {
            currentState = GameState::INTRO;
            stateTimer = 0.f;
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            window.close();
        }
    }
}

void Game::updateMenu(float dt) {
    stateTimer += dt;
    if (stateTimer > 2.0f) {
        currentState = GameState::INTRO;
        stateTimer = 0.f;
    }
}

void Game::renderMenu() {
    debugText.setString("MENU\nPress ENTER to start\nPress ESCAPE to exit");
    window.draw(debugText);
}

// ------- INTRO -------
void Game::updateIntro(float dt) {
    stateTimer += dt;
    if (stateTimer > 2.0f) {
        currentState = GameState::TUTORIAL;
        stateTimer = 0.f;
    }
}

void Game::renderIntro() {
    debugText.setString("INTRO...");
    window.draw(debugText);
}

void Game::processIntroEvents(const sf::Event&) {}

// ------- TUTORIAL -------
void Game::processTutorialEvents(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                currentState = GameState::PLAYING;
                stateTimer = 0.f;
            }
        }
    }
}

void Game::updateTutorial(float dt) {
    stateTimer += dt;
    if (stateTimer > 2.0f) {
        currentState = GameState::PLAYING;
        stateTimer = 0.f;
    }
}

void Game::renderTutorial() {
    debugText.setString("TUTORIAL\nPress SPACE to continue");
    window.draw(debugText);
}

// ------- PLAYING -------
void Game::processPlayingEvents(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::W) {
            currentState = GameState::WIN;
            stateTimer = 0.f;
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::L) {
            currentState = GameState::LOSE;
            stateTimer = 0.f;
        }
    }
}

void Game::updatePlaying(float dt) {    
    // THIS is where your real game logic will go
}

void Game::renderPlaying() {
    debugText.setString("PLAYING\nPress W = WIN, L = LOSE");
    window.draw(debugText);
}

// ------- WIN -------
void Game::processWinEvents(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
    }
}

void Game::updateWin(float dt) {
    stateTimer += dt;
    if (stateTimer > 2.0f) {
        currentState = GameState::MENU;
        stateTimer = 0.f;
    }
}

void Game::renderWin() {
    debugText.setString("YOU WIN\nPress any key");
    window.draw(debugText);
}

// ------- LOSE -------
void Game::processLoseEvents(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
    }
}

void Game::updateLose(float dt) {
    stateTimer += dt;
    if (stateTimer > 2.0f) {
        currentState = GameState::MENU;
        stateTimer = 0.f;
    }
}

void Game::renderLose() {
    debugText.setString("YOU LOSE\nPress any key");
    window.draw(debugText);
}