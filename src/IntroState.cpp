#include "IntroState.h"
#include "NewGame.h"
#include "TutorialState.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <memory>

namespace {
constexpr float kAlphaMax = 25.f;
constexpr float kIntroScrollStartSeconds = 0.0f;
}
IntroState::IntroState(Game& game)
: State(game),
  introText(game.getFont()),
  stateTimer(0.0f),
  introTextY(0.0f),
  introScrollSpeed(60.0f),
  introBackgroundFadeAlpha(kAlphaMax),
  introBackgroundFadeSeconds(0.01f),
  introFadeOverlay(),
  introTextContent("")
{
    introText.setCharacterSize(36);
    introText.setFillColor(sf::Color::White);
}

void IntroState::onEnter(){
    loadIntroTextFromFile();
    resetIntroSequence();
};

void IntroState::onExit(){};

BackgroundId IntroState::getBackgroundId() const
{
    return BackgroundId::INTRO;
}

void IntroState::handleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Space
            || keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
            game.changeState(std::make_unique<TutorialState>(game));
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            game.getWindow().close();
        }
    }
}

void IntroState::update(float dt)
{
    stateTimer += dt;

    const float fadeDuration = std::max(0.001f, introBackgroundFadeSeconds);
    const float fadeProgress = std::clamp(stateTimer / fadeDuration, 0.0f, 1.0f);
    introBackgroundFadeAlpha = kAlphaMax * (1.0f - fadeProgress);

    if (stateTimer >= kIntroScrollStartSeconds) {
        introTextY -= introScrollSpeed * dt;
        introText.setPosition({700.f, introTextY});
    }
    introText.setFillColor(sf::Color::White);

    const float textHeight = introText.getLocalBounds().size.y;
    if (introTextY + textHeight < 0.0f) {
        game.changeState(std::make_unique<TutorialState>(game));
    }
}

void IntroState::render(sf::RenderWindow& window)
{
    window.draw(introText);
    introFadeOverlay.setSize(sf::Vector2f(game.getWindow().getSize()));
    introFadeOverlay.setFillColor(sf::Color(
        0,
        0,
        0,
        static_cast<std::uint8_t>(std::clamp(introBackgroundFadeAlpha, 0.0f, kAlphaMax))
    ));
    window.draw(introFadeOverlay);
}

void IntroState::resetIntroSequence()
{
    introBackgroundFadeAlpha = kAlphaMax;
    introText.setString(introTextContent);
    const float screenHeight = static_cast<float>(game.getWindow().getSize().y);
    introTextY = screenHeight + 20.0f;
    introText.setPosition({500.f, introTextY});
    introText.setFillColor(sf::Color::White);
}

void IntroState::loadIntroTextFromFile() {
    std::ifstream file("assets/intro/intro.txt");
    if (!file.is_open()) {
        introTextContent = "INTRO TEXT FILE NOT FOUND\n\nCreate assets/intro/intro.txt";
        return;
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    introTextContent = buffer.str();
}