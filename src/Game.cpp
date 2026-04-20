#include "Game.h"
#include "Enemy.h"
#include "Capsule.h"
#include "Projectiles.h"
#include <algorithm>
#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>

namespace {
constexpr int kEnemySeparationPasses = 3;
constexpr float kProjectileSpeed = 4500.f;
/// Only place to tune on-screen projectile size (sprite scale factor). Ignores defaults in Projectiles.h.
constexpr float kProjectileVisualScale = 8.f;
constexpr float kAlphaMax = 25.f;
constexpr float kIntroScrollStartSeconds = 0.0f;

/// Resolves enemy overlaps
void resolveEnemyOverlaps(std::vector<Enemy>& enemies) {
    for (int pass = 0; pass < kEnemySeparationPasses; ++pass) {
        for (std::size_t i = 0; i < enemies.size(); ++i) {
            if (!enemies[i].isAlive()) {
                continue;
            }
            for (std::size_t j = i + 1; j < enemies.size(); ++j) {
                if (!enemies[j].isAlive()) {
                    continue;
                }
                const sf::FloatRect a = enemies[i].getGlobalBounds();
                const sf::FloatRect b = enemies[j].getGlobalBounds();
                const auto intersection = a.findIntersection(b);
                if (!intersection.has_value()) {
                    continue;
                }

                const float overlapX = intersection->size.x;
                const float overlapY = intersection->size.y;
                const sf::Vector2f cA = a.getCenter();
                const sf::Vector2f cB = b.getCenter();

                if (overlapX < overlapY) {
                    const float half = overlapX * 0.5f;
                    if (cA.x < cB.x) {
                        enemies[i].translate({-half, 0.f});
                        enemies[j].translate({half, 0.f});
                    } else {
                        enemies[i].translate({half, 0.f});
                        enemies[j].translate({-half, 0.f});
                    }
                } else {
                    const float half = overlapY * 0.5f;
                    if (cA.y < cB.y) {
                        enemies[i].translate({0.f, -half});
                        enemies[j].translate({0.f, half});
                    } else {
                        enemies[i].translate({0.f, half});
                        enemies[j].translate({0.f, -half});
                    }
                }
            }
        }
    }
}

/// Resolves enemy capsule overlap
void resolveEnemyCapsuleOverlap(std::vector<Enemy>& enemies, Capsule& capsule) {
    if (!capsule.isAlive()) {
        return;
    }
    const sf::FloatRect capBounds = capsule.getGlobalBounds();
    const sf::Vector2f capCenter = capBounds.getCenter();

    for (Enemy& enemy : enemies) {
        if (!enemy.isAlive()) {
            continue;
        }
        const sf::FloatRect eBounds = enemy.getGlobalBounds();
        const auto intersection = eBounds.findIntersection(capBounds);
        if (!intersection.has_value()) {
            continue;
        }

        const float overlapX = intersection->size.x;
        const float overlapY = intersection->size.y;
        const sf::Vector2f enemyCenter = eBounds.getCenter();

        if (overlapX < overlapY) {
            const float pen = overlapX;
            if (enemyCenter.x < capCenter.x) {
                enemy.translate({-pen, 0.f});
            } else {
                enemy.translate({pen, 0.f});
            }
        } else {
            const float pen = overlapY;
            if (enemyCenter.y < capCenter.y) {
                enemy.translate({0.f, -pen});
            } else {
                enemy.translate({0.f, pen});
            }
        }
    }
}

/// Resolves enemy separations
void resolveEnemySeparations(std::vector<Enemy>& enemies, Capsule& capsule) {
    for (int pass = 0; pass < kEnemySeparationPasses; ++pass) {
        resolveEnemyOverlaps(enemies);
        resolveEnemyCapsuleOverlap(enemies, capsule);
    }
}
} // namespace

Game::Game()
    : window(sf::VideoMode({1920, 1080}), "Game"),
    currentState(GameState::LOADING), // Initial state
    debugText(font),
    capsuleHealthText(font),
    capsuleEnergyText(font),
    introText(font),
    stateTimer(0.0f), // Example timer (useful for intro/loading screens)
    introTextY(0.0f), // Initial y position of the intro text
    introScrollSpeed(30.0f),
    introFadeTopFraction(0.25f),
    introFadeBottomFraction(0.75f),
    introBackgroundFadeAlpha(kAlphaMax), // Initial alpha value for the intro background fade overlay
    introBackgroundFadeSeconds(0.01f), // Time it takes for the intro background fade overlay to fade
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

    capsuleHealthText.setFont(font);
    capsuleHealthText.setCharacterSize(96);
    capsuleHealthText.setFillColor(sf::Color::Red);
    capsuleHealthText.setPosition({40.f, 20.f});

    capsuleEnergyText.setFont(font);
    capsuleEnergyText.setCharacterSize(96);
    capsuleEnergyText.setFillColor(sf::Color::Blue);
    capsuleEnergyText.setPosition({40.f, 130.f});

    introText.setFont(font);
    introText.setCharacterSize(36);
    introText.setFillColor(sf::Color::White);
    introText.setPosition({80.f, 0.f});
    introFadeOverlay.setPosition({0.f, 0.f});
    introFadeOverlay.setFillColor(sf::Color::Black);

    loadIntroTextFromFile();
    loadBackgroundTextures();
    resetIntroSequence();

    turret.setPosition({520.f, 750.f});
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
    stateTimer += dt;

    // Simple timer-driven fade: fully black at t=0, fully clear at t=introBackgroundFadeSeconds.
    const float fadeDuration = std::max(0.001f, introBackgroundFadeSeconds);
    const float fadeProgress = std::clamp(stateTimer / fadeDuration, 0.0f, 1.0f);
    introBackgroundFadeAlpha = kAlphaMax * (1.0f - fadeProgress);

    // Scroll start is explicitly timer-gated for easy tuning.
    if (stateTimer >= kIntroScrollStartSeconds) {
        introTextY -= introScrollSpeed * dt;
        introText.setPosition({80.f, introTextY});
    }
    introText.setFillColor(sf::Color::White);

    const float textHeight = introText.getLocalBounds().size.y;
    if (introTextY + textHeight < 0.0f) { // If the y position of the intro text is less than 0
        currentState = GameState::TUTORIAL; // Set the current state to the tutorial state
        stateTimer = 0.f; // Set the state timer to 0
    }
}

void Game::renderIntro() { // Render the intro
    window.draw(introText); // Draw intro while background overlay fades
    introFadeOverlay.setSize(sf::Vector2f(window.getSize())); // Set the size of the intro fade overlay to the size of the window
    introFadeOverlay.setFillColor(sf::Color(
        0,
        0,
        0,
        static_cast<std::uint8_t>(std::clamp(introBackgroundFadeAlpha, 0.0f, kAlphaMax)) // Update the alpha of the intro fade overlay
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
    introBackgroundFadeAlpha = kAlphaMax; // Set the alpha of the intro background fade overlay to max
    introText.setString(introTextContent); // Set the string of the intro text to the intro text content    
    const float screenHeight = static_cast<float>(window.getSize().y); // Get the height of the window
    introTextY = screenHeight + 20.0f; // Set the y position of the intro text to the screen height + 20
    introText.setPosition({500.f, introTextY}); // Set the position of the intro text to 80, introTextY
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

                capsule.setCapsuleEnergy(0.0f);
                enemies.clear();
                projectiles.clear();
                nextSpawnIndex = 0;
                spawner.setPosition({1700.f, 300.f});
            }
        }
    }
}

void Game::updateTutorial(float dt) { // Update the tutorial
    (void)dt;
}

void Game::renderTutorial() { // Render the tutorial
    spawner.draw(window);
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
        if (keyPressed->scancode == sf::Keyboard::Scancode::P) {
            targetPriority = (targetPriority == TargetPriority::Random) ? TargetPriority::First : TargetPriority::Random;
        }
    }
}

// updates every enemy on screen
void Game::updatePlaying(float dt) { // Update the playing
    const sf::Vector2f capPos = capsule.getPosition();

    for (auto& enemy : enemies) {
        enemy.setMoveGoal(capPos);
    }
    for (auto& enemy : enemies) {
        enemy.update(dt);
    }

    spawner.update(dt);
    while (spawner.shouldSpawn()) {
        spawner.consumeSpawn();
        enemies.emplace_back(80.f, 80.f, 8.f, 1.f, 0.f, 120.f, spawner.getPosition(), capPos, nextSpawnIndex);
        ++nextSpawnIndex;
    }

    // Melee while AABBs can overlap; separation below removes overlap so this must run first.
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            enemy.tryMeleeAttack(capsule);
        }
    }

    resolveEnemySeparations(enemies, capsule);

    std::vector<Enemy*> inCapsuleRange;
    inCapsuleRange.reserve(enemies.size());
    const float capRange = capsule.getAttackRange();
    const float capRangeSq = capRange * capRange;
    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) {
            continue;
        }
        const sf::Vector2f d = enemy.getPosition() - capPos;
        if (d.x * d.x + d.y * d.y <= capRangeSq) {
            inCapsuleRange.push_back(&enemy);
        }
    }

    capsule.resolveAttackTarget(inCapsuleRange, targetPriority);

    Enemy* capTgt = capsule.getAttackTarget();
    sf::Vector2f capsuleShotEnd{};
    if (capTgt != nullptr && capTgt->isAlive()) {
        capsuleShotEnd = capTgt->getPosition();
    }
    if (capsule.attack()) {
        projectiles.emplace_back(capsule.getPosition(), capsuleShotEnd, kProjectileSpeed,
                                 sf::Vector2f{kProjectileVisualScale, kProjectileVisualScale});
    }

    turret.setAttackTarget(capsule.getAttackTarget());
    turret.update(dt);

    Enemy* turTgt = turret.getAttackTarget();
    sf::Vector2f turretShotEnd{};
    if (turTgt != nullptr && turTgt->isAlive()) {
        turretShotEnd = turTgt->getPosition();
    }
    if (turret.attack()) {
        projectiles.emplace_back(turret.getPosition(), turretShotEnd, kProjectileSpeed,
                                 sf::Vector2f{kProjectileVisualScale, kProjectileVisualScale});
    }

    for (Projectile& p : projectiles) {
        p.update(dt);
    }
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
                                     [](const Projectile& p) { return !p.isActive(); }),
                      projectiles.end());

    if (capsule.getAttackTarget() != nullptr && !capsule.getAttackTarget()->isAlive()) {
        capsule.clearAttackTarget();
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](const Enemy& e) { return !e.isAlive(); }),
                  enemies.end());

    if (!capsule.isAlive()) {
        currentState = GameState::LOSE;
        stateTimer = 0.f;
    } else if (capsule.isFullyCharged()) {
        currentState = GameState::WIN;
        stateTimer = 0.f;
    }
}

void Game::renderPlaying() { // Render the playing  
   // capsule.setVisualScale({0.1f, 0.1f}); // scales sprite + hitbox together
    spawner.draw(window);
    capsule.draw(window);
    turret.draw(window);

    capsuleHealthText.setString(std::to_string(static_cast<int>(capsule.getHealth())));
    window.draw(capsuleHealthText);

    capsuleEnergyText.setString(std::to_string(static_cast<int>(capsule.getEnergy())));
    window.draw(capsuleEnergyText);

    const char* pri = (targetPriority == TargetPriority::Random) ? "Random" : "First";
    debugText.setString(std::string("PLAYING\nPress W = WIN, L = LOSE, P = toggle target priority (") + pri + ")");
    window.draw(debugText); // Draw the debug text

    for (auto& enemy : enemies) {
        enemy.draw(window);
    }

    for (Projectile& p : projectiles) {
        p.draw(window);
    }
    
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