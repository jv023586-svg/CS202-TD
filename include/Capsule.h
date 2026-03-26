#pragma once
#include <SFML/Graphics.hpp>

class Capsule {
public:
    Capsule();

    void update();
    void draw(sf::RenderWindow& window);
};