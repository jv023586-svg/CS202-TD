#pragma once
#include "State.h"

class MenuState : public State {
public:
    explicit MenuState(Game& game);

    void onEnter() override;
    void onExit() override;

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    BackgroundId getBackgroundId() const override;

private:
    sf::Text text;
    // I really want to add a button. If I did, it would start here
};