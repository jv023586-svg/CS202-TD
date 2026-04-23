#pragma once
#include "State.h"

class LoadingState : public State {
public:
    explicit LoadingState(Game& game);

    void onEnter() override;
    void onExit() override;

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    BackgroundId getBackgroundId() const override;

private:
    sf::Text text;
    float stateTimer;
};