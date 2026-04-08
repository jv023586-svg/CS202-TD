#include "Game.h"

Game::Game()
    : window(sf::VideoMode({800, 600}), "Game")
{
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

void Game::update() {
    capsule.update();
}

void Game::render() {
    window.clear();
    capsule.draw(window);
    window.display();
}