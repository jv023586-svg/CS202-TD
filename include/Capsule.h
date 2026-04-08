#pragma once
#include <SFML/Graphics.hpp>

class Capsule {
public:
    Capsule(); // Constructor
    
    void update(); // Updates the position of the capsule
    void draw(sf::RenderWindow& window); // Draws the capsule to the screen
};