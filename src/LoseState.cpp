#include "LoseState.h"
#include "NewGame.h"
#include "MenuState.h"
#include <memory>

namespace {
constexpr float kTextPaddingPx = 10.f;
}

LoseState::LoseState(Game& game)
    : State(game), text(game.getFont()) {
    text.setCharacterSize(48);
    text.setFillColor(sf::Color::Red);
    text.setString("YOU LOSE\nPress ENTER for main menu\nPress ESCAPE to quit");
}

void LoseState::onEnter() {
}

void LoseState::onExit() {
}

void LoseState::handleEvent(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
            game.changeState(std::make_unique<MenuState>(game));
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            game.getWindow().close();
        }
    }
}

void LoseState::update(float) {
}

void LoseState::render(sf::RenderWindow& window) {
    const auto b = text.getLocalBounds();
    text.setOrigin({b.size.x * 0.5f, b.size.y * 0.5f});
    text.setPosition({
        game.getWindow().getSize().x * 0.5f,
        game.getWindow().getSize().y * 0.5f
    });

    const sf::FloatRect gb = text.getGlobalBounds();
    sf::RectangleShape background;
    background.setPosition(gb.position - sf::Vector2f{kTextPaddingPx, kTextPaddingPx});
    background.setSize({gb.size.x + 2.f * kTextPaddingPx, gb.size.y + 2.f * kTextPaddingPx});
    background.setFillColor(sf::Color::Black);
    window.draw(background);
    window.draw(text);
}

BackgroundId LoseState::getBackgroundId() const {
    return BackgroundId::GAMEPLAY;
}
