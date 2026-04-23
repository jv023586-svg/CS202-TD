#pragma once
#include "State.h"

class IntroState : public State {
public:
    explicit IntroState(Game& game);

    void onEnter() override;
    void onExit() override;

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    BackgroundId getBackgroundId() const override;
    

private:
    sf::Text introText;
    float stateTimer;
    float introTextY;
    float introScrollSpeed;
    float introBackgroundFadeAlpha;
    float introBackgroundFadeSeconds;
    sf::RectangleShape introFadeOverlay;
    std::string introTextContent;
    
    void resetIntroSequence();
    void loadIntroTextFromFile();
};