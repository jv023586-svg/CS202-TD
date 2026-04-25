#include "MenuState.h"
#include "NewGame.h"
#include "IntroState.h"
#include <memory>

MenuState::MenuState(Game& game)
: State(game), text(game.getFont())
{
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::White);
    text.setString("MENU\nPress ENTER to start\nPress ESCAPE to exit");
    text.setPosition({100.f, 250.f});
}

void MenuState::onEnter(){};

void MenuState::onExit(){};

BackgroundId MenuState::getBackgroundId() const
{
    return BackgroundId::MENU;
}

void MenuState::handleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
            game.changeState(std::make_unique<IntroState>(game));
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            game.getWindow().close();
        }
    }
}

void MenuState::update(float dt)
{
    (void)dt;
}

void MenuState::render(sf::RenderWindow& window)
{
    window.draw(text);
}