#pragma once
#include <SFML/Graphics.hpp>
#include "BackgroundId.h"
#include "NewGame.h"

class State {
public:
    explicit State(Game& game) : game(game) {} // Constructor
    virtual ~State() = default; // Destructor
    
    virtual void onEnter() = 0;
    virtual void onExit() = 0;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float dt) = 0; 
    virtual void render(sf::RenderWindow& window) = 0;
    
    virtual BackgroundId getBackgroundId() const = 0;

protected:
    Game& game;
};