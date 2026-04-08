#pragma once

#include <SFML/Graphics.hpp>
#include "Capsule.h"

class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    Capsule capsule;

    void processEvents();
    void update();
    void render();
};