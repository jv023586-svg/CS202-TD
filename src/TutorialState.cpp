#include "TutorialState.h"
#include "NewGame.h"
#include "PlayingState.h"
#include <memory>

namespace {
constexpr float kTextPaddingPx = 6.f;

void drawTextWithBlackBacking(sf::RenderWindow& window, const sf::Text& text)
{
    const sf::FloatRect bounds = text.getGlobalBounds();
    sf::RectangleShape background;
    background.setPosition(bounds.position - sf::Vector2f{kTextPaddingPx, kTextPaddingPx});
    background.setSize({bounds.size.x + 2.f * kTextPaddingPx, bounds.size.y + 2.f * kTextPaddingPx});
    background.setFillColor(sf::Color::Black);
    window.draw(background);
    window.draw(text);
}
} // namespace

TutorialState::TutorialState(Game& game)
: State(game),
  capsule(),
  spawner(),
  enemy(200.f, 200.f, 10.f, 1.f, 1.f, 0.f, sf::Vector2f(game.getWindow().getSize().x / 2.f, game.getWindow().getSize().y / 3.f), sf::Vector2f(110.f, 110.f)),
  TitleText(game.getFont()),
  CapsuleText(game.getFont()),
  SpawnerText(game.getFont()),
  HealthText(game.getFont()),
  EnergyText(game.getFont()),
  EnemyText(game.getFont())
{
    TitleText.setCharacterSize(48);
    TitleText.setFillColor(sf::Color::White);
    TitleText.setString("TUTORIAL");
    const auto tb = TitleText.getLocalBounds();
    TitleText.setOrigin({tb.size.x * 0.5f, tb.size.y * 0.5f});
    TitleText.setPosition({
        game.getWindow().getSize().x / 2.f,
        game.getWindow().getSize().y / 2.f});

    CapsuleText.setCharacterSize(24);
    CapsuleText.setFillColor(sf::Color::White);
    CapsuleText.setString("This is your Capsule.\nCapsule has health and energy.\nCapsule can attack enemies.\nCapsule cannot move.");
    const auto cb = CapsuleText.getLocalBounds();
    CapsuleText.setOrigin({cb.size.x * 0.5f, cb.size.y * 0.5f});
    CapsuleText.setPosition({
        capsule.getPosition().x + 300.f,
        capsule.getPosition().y + 0.f});

    SpawnerText.setCharacterSize(24);
    SpawnerText.setFillColor(sf::Color::White);
    SpawnerText.setString("This is the Spawner.\nSpawner spawns enemies.\nSpawner cannot move.");
    const auto sb = SpawnerText.getLocalBounds();
    SpawnerText.setOrigin({sb.size.x * 0.5f, sb.size.y * 0.5f});
    SpawnerText.setPosition({
        spawner.getPosition().x,
        spawner.getPosition().y + 100.f});

    HealthText.setCharacterSize(40);
    HealthText.setFillColor(sf::Color::Red);
    HealthText.setString("Health: " + std::to_string(static_cast<int>(capsule.getHealth())) + "\nIf your health reaches 0, you will lose the game.");
    const auto hb = HealthText.getLocalBounds();
    HealthText.setOrigin({hb.size.x * 0.5f, hb.size.y * 0.5f});
    HealthText.setPosition({
        game.getWindow().getSize().x / 4.f,
        game.getWindow().getSize().y / 8.f});

    EnergyText.setCharacterSize(36);
    EnergyText.setFillColor(sf::Color(0, 201, 255)); // brighter blue
    EnergyText.setString("Energy: " + std::to_string(static_cast<int>(capsule.getEnergy())) + "\nCapsule will automatically recharge energy over time.\nIf Energy reaches 100, you win!");
    const auto eb = EnergyText.getLocalBounds();
    EnergyText.setOrigin({eb.size.x * 0.5f, eb.size.y * 0.5f});
    EnergyText.setPosition({
        (game.getWindow().getSize().x / 4.f) * 3,
        game.getWindow().getSize().y / 8.f});

    EnemyText.setCharacterSize(24);
    EnemyText.setFillColor(sf::Color::White);
    EnemyText.setString("This is the Enemy.\nEnemy has health and will attack your Capsule.\nEnemy will move towards your Capsule.\nEnemy will attack your Capsule if it is in range.");
    const auto enb = EnemyText.getLocalBounds();
    EnemyText.setOrigin({enb.size.x * 0.5f, enb.size.y * 0.5f});
    EnemyText.setPosition({
        enemy.getPosition().x,
        enemy.getPosition().y + 85.f});

    stateTimer = 0.0f;
}

void TutorialState::onEnter()
{
}

void TutorialState::onExit()
{
}

BackgroundId TutorialState::getBackgroundId() const
{
    return BackgroundId::GAMEPLAY;
}
void TutorialState::handleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
            game.changeState(std::make_unique<PlayingState>(game));
        }
    }
}

void TutorialState::update(float dt)
{
    stateTimer += dt;
    if (stateTimer > 30.0f) {
        game.changeState(std::make_unique<PlayingState>(game));
    }
}

void TutorialState::render(sf::RenderWindow& window)
{
    displayTitleText();
    if (stateTimer > 2.0f) {
        capsule.draw(window);
        displayCapsuleText();
    }
    if (stateTimer > 6.0f) {
        displayHealthText();
    }
    if (stateTimer > 10.0f) {
        displayEnergyText();
    }
    if (stateTimer > 14.0f) {
        spawner.draw(window);
        displaySpawnerText();
    }
    if (stateTimer > 18.0f) {
        enemy.draw(window);
        displayEnemyText();
    }
}

void TutorialState::displayTitleText()
{
    drawTextWithBlackBacking(game.getWindow(), TitleText);
}

void TutorialState::displayCapsuleText()
{
    drawTextWithBlackBacking(game.getWindow(), CapsuleText);
}

void TutorialState::displaySpawnerText()
{
    drawTextWithBlackBacking(game.getWindow(), SpawnerText);
}

void TutorialState::displayHealthText()
{
    drawTextWithBlackBacking(game.getWindow(), HealthText);
}

void TutorialState::displayEnergyText()
{
    drawTextWithBlackBacking(game.getWindow(), EnergyText);
}

void TutorialState::displayEnemyText()
{
    drawTextWithBlackBacking(game.getWindow(), EnemyText);
}
