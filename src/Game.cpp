#include "Game.h"

Game::Game()
    : window(sf::VideoMode({800, 600}), "Game")
{
}

void Game::run() {
    while (window.isOpen()) {
        processEvents(); // Handles window events, such as closing the window and user input
        update(); // Updates the game state, such as the position of the objects
        render(); // Renders the game state to the screen
    }
}

void Game::processEvents() {
    while (const std::optional event = window.pollEvent()) { // PollEvent is a function that checks for events, such as closing the window and user input
        if (event->is<sf::Event::Closed>()) {
            window.close(); // Closes the window
        }
    }
}

void Game::update() {// Updates the game state, such as the position of the objects
    capsule.update(); // Updates the position of the capsule
}

void Game::render() {
    window.clear(); // Clears the screen
    capsule.draw(window); // Draws the capsule to the screen, using the draw function from the Capsule class
    window.display(); // Displays the screen
}