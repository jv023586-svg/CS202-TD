#pragma once
#include "State.h"

class TutorialState : public State {
public:
    explicit TutorialState(Game& game);

    void onEnter() override;
    void onExit() override;

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    BackgroundId getBackgroundId() const override;

private:
    sf::Text TitleText;
    sf::Text CapsuleText;
    sf::Text SpawnerText;
    sf::Text HealthText;
    sf::Text EnergyText;
    sf::Text EnemyText;

    void displayTitleText();
    void displayCapsuleText();
    void displaySpawnerText();
    void displayHealthText();
    void displayEnergyText();
    void displayEnemyText();
};