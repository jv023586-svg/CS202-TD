#include "NewGame.h"
#include "State.h"
#include "LoadingState.h"
#include "BackgroundId.h"
#include <optional>
#include <stdexcept>
#include <iostream>
#include <array>

Game::Game()
: window(sf::VideoMode({1920, 1080}), "Game")
{
    window.setFramerateLimit(60);

    if (!font.openFromFile("assets/fonts/Arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
    loadBackgroundTextures();
    // Set the initial state to the loading state
    changeState(std::make_unique<LoadingState>(*this)); 
}

void Game::run()
{
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        handleEvents();
        update(dt);
        render();
    }
}

// Function to change the current state via a unique pointer to the new state
void Game::changeState(std::unique_ptr<State> newState)
{
    if (currentState) currentState->onExit();
    currentState = std::move(newState);
    if (currentState) currentState->onEnter();
}

sf::RenderWindow& Game::getWindow()
{
    return window;
}

sf::Font& Game::getFont()
{
    return font;
}

void Game::handleEvents()
{
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close(); 
        }

        if (currentState) {
            currentState->handleEvent(*event);
        }
    }
}

void Game::update(float dt)
{
    if (currentState) {
        currentState->update(dt);
    }
}

void Game::render()
{
    window.clear();
    if (currentState) {
        drawBackgroundForState(currentState->getBackgroundId());
        currentState->render(window);
    }
    window.display();
}

void Game::drawBackgroundForState(BackgroundId backgroundId)
{
    const std::size_t index = static_cast<std::size_t>(backgroundId);

    if (index >= backgroundTextures.size() || !backgroundLoaded[index]) return;

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTextures[index]);

    const auto textureSize = backgroundTextures[index].getSize();
    if (textureSize.x == 0 || textureSize.y == 0) return;

        backgroundSprite.setScale({
            static_cast<float>(window.getSize().x) / static_cast<float>(textureSize.x),
            static_cast<float>(window.getSize().y) / static_cast<float>(textureSize.y)
        });
        
    window.draw(backgroundSprite);
}

void Game::loadBackgroundTextures()
{
    static constexpr std::array<const char*, kBackgroundCount> paths = {
        "assets/backgrounds/loading.png",
        "assets/backgrounds/menu.png",
        "assets/backgrounds/intro.png",
        "assets/backgrounds/gameplay.png"
    };
    // Error handling for loading the background textures
    for (std::size_t i = 0; i < paths.size(); ++i) {
        backgroundLoaded[i] = backgroundTextures[i].loadFromFile(paths[i]);
        if (!backgroundLoaded[i]) {
            std::cerr << "Failed to load background: " << paths[i] << '\n';
        }
    }
}
