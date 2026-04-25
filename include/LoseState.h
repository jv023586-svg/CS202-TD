#pragma once

#include "State.h"

class LoseState : public State {
public:
    explicit LoseState(Game& game);

    void onEnter() override;
    void onExit() override;
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    BackgroundId getBackgroundId() const override;

private:
    sf::Text text;
};
