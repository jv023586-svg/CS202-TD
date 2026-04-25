#include "LoadingState.h"
#include "NewGame.h"
#include "MenuState.h"

#include <memory>

LoadingState::LoadingState(Game& game)
: State(game), text(game.getFont()), stateTimer(0.f)
{
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::White);
    text.setString("Loading...");
    text.setPosition({100.f, 250.f});
}

void LoadingState::onEnter(){};

void LoadingState::onExit(){};

BackgroundId LoadingState::getBackgroundId() const
{
    return BackgroundId::LOADING;
}

void LoadingState::handleEvent(const sf::Event& event)
{
    (void)event;
}

void LoadingState::update(float dt)
{
    stateTimer += dt;
    if (stateTimer > 3.0f) {
        game.changeState(std::make_unique<MenuState>(game));
    }
}

void LoadingState::render(sf::RenderWindow& window)
{
    window.draw(text);
}