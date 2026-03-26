#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy();

    void update();
    void draw(sf::RenderWindow& window);
};