#include "Game.h"
#include <SFML/Graphics.hpp>

void Game::run() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Game");

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}