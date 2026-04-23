#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "BackgroundId.h"
#include <array>

class State;

class Game {
public:
    Game();
    void run();

    void changeState(std::unique_ptr<State> newState);

    sf::RenderWindow& getWindow();
    sf::Font& getFont();

private:
    void handleEvents();
    void update(float dt);
    void render();

    void drawBackgroundForState(BackgroundId backgroundId);
    void loadBackgroundTextures();

    sf::RenderWindow window;
    sf::Font font;
    
    std::unique_ptr<State> currentState;
    
    static constexpr std::size_t kBackgroundCount = 4u;
    std::array<sf::Texture, kBackgroundCount> backgroundTextures;
    std::array<bool, kBackgroundCount> backgroundLoaded{};
};