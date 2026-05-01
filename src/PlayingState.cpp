#include "PlayingState.h"
#include "NewGame.h"
#include "Capsule.h"
#include "Spawner.h"
#include "Enemy.h"
#include "Projectiles.h"
#include "Turret.h"
#include "TargetPriority.h"
#include "WinState.h"
#include "LoseState.h"
#include <algorithm>

namespace {
constexpr float kProjectileSpeed = 4500.f;
constexpr float kProjectileVisualScale = 8.f;
constexpr float kTextPaddingPx = 6.f;

void drawTextWithWhiteBacking(sf::RenderWindow& w, const sf::Text& text)
{
    const sf::FloatRect gb = text.getGlobalBounds();
    sf::RectangleShape background;
    background.setPosition(gb.position - sf::Vector2f{kTextPaddingPx, kTextPaddingPx});
    background.setSize({gb.size.x + 2.f * kTextPaddingPx, gb.size.y + 2.f * kTextPaddingPx});
    background.setFillColor(sf::Color::Black);
    w.draw(background);
    w.draw(text);
}
} // namespace

PlayingState::PlayingState(Game& game)
: State(game),
  HealthText(game.getFont()),
  EnergyText(game.getFont()),
  TargetPriorityText(game.getFont()),
  TurretHintText(game.getFont())
{
    turret.setPosition({capsule.getPosition().x, capsule.getPosition().y - 150.f});
    turret.setTurretVisualScale({4.f, 4.f});

    HealthText.setCharacterSize(36);
    HealthText.setFillColor(sf::Color::Red);
    HealthText.setString("Health: " + std::to_string(static_cast<int>(capsule.getHealth())));
    const auto hb = HealthText.getLocalBounds();
    HealthText.setOrigin({hb.size.x * 0.5f, hb.size.y * 0.5f});
    HealthText.setPosition({
        game.getWindow().getSize().x / 4.f,
        game.getWindow().getSize().y / 8.f});

    EnergyText.setCharacterSize(36);
    EnergyText.setFillColor(sf::Color(0, 201, 255)); // brighter blue
    EnergyText.setString("Energy: " + std::to_string(static_cast<int>(capsule.getEnergy())));
    const auto eb = EnergyText.getLocalBounds();
    EnergyText.setOrigin({eb.size.x * 0.5f, eb.size.y * 0.5f});
    EnergyText.setPosition({
        (game.getWindow().getSize().x / 4.f) * 3,
        game.getWindow().getSize().y / 8.f});

    TargetPriorityText.setCharacterSize(28);
    TargetPriorityText.setFillColor(sf::Color(220, 220, 100));

    TurretHintText.setCharacterSize(34);
    TurretHintText.setFillColor(sf::Color::Red);
    TurretHintText.setString("You're being overwhelmed! Quick, place a turret by pressing 'T'");

    stateTimer = 0.0f;
}

void PlayingState::onEnter()
{
}

void PlayingState::onExit()
{
}

BackgroundId PlayingState::getBackgroundId() const
{
    return BackgroundId::GAMEPLAY;
}

void PlayingState::handleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::P) {
            targetPriority = (targetPriority == TargetPriority::Oldest) ? TargetPriority::Newest
                                                                         : TargetPriority::Oldest;
        }
        if (keyPressed->scancode == sf::Keyboard::Scancode::T && turretHintTriggered) {
            turretPlaced = true;
        }
    }
    
}

void PlayingState::update(float dt)
{
    capsule.update(dt);
    spawner.update(dt);
    const sf::Vector2f capPos = capsule.getPosition();
    while (spawner.shouldSpawn()) {
        spawner.consumeSpawn();
        // Spawn enemy at spawner with a movement goal toward capsule.
        TargetList.emplace_back(80.f, 80.f, 8.f, 1.f, 0.f, 120.f,
                                spawner.getPosition(), capPos, nextSpawnIndex);
        ++nextSpawnIndex;
    }
    for (auto& e : TargetList) { // For every enemy in the TargetList, update their destination
        e.setMoveGoal(capPos);
    }
    for (auto& e : TargetList) { // update every enemy in the TargetList
        e.update(dt);
    }

    // Melee while AABBs can overlap; separation below removes overlap so this must run first.
    for (auto& e : TargetList) { // Check to see if an enemy in TargetList can melle the capsule
        if (e.isAlive()) {
            e.tryMeleeAttack(capsule);
        }
    }
    // Function call to select a target from the list.
    Enemy* selectedTarget = selectTargetFromTargetList();
    capsule.setAttackTarget(selectedTarget);
    if (capsule.attack() && selectedTarget != nullptr) {
        ProjectileList.emplace_back(
            capsule.getPosition(),
            selectedTarget->getPosition(),
            kProjectileSpeed,
            sf::Vector2f{kProjectileVisualScale, kProjectileVisualScale}
        );
    }
    if (turretPlaced) {
        turret.setAttackTarget(capsule.getAttackTarget());
        turret.update(dt);

        Enemy* turretTarget = turret.getAttackTarget();
        if (turret.attack() && turretTarget != nullptr) {
            ProjectileList.emplace_back(
                turret.getPosition(),
                turretTarget->getPosition(),
                kProjectileSpeed,
                sf::Vector2f{kProjectileVisualScale, kProjectileVisualScale}
            );
        }
    }

    resolveCollisions();

    for (auto& projectile : ProjectileList) {
        projectile.update(dt);
    }
    // Use remove_if to remove all inactive projectiles from the ProjectileList
    ProjectileList.remove_if([](const Projectile& projectile) { return !projectile.isActive(); });
    // If the capsule's attack target is not alive, clear the attack target
    if (capsule.getAttackTarget() != nullptr && !capsule.getAttackTarget()->isAlive()) {
        capsule.clearAttackTarget();
    }
    // use remove_if to remove all inactive enemies from the TargetList
    TargetList.remove_if([](const Enemy& enemy) { return !enemy.isAlive(); });
    if (!turretHintTriggered && capsule.getHealth() < 300.f) {
        turretHintTriggered = true;
    }
    stateTimer += dt;
    checkGameOver();
    checkGameWon();
}

void PlayingState::render(sf::RenderWindow& window)
{
    displayHealthText();
    displayEnergyText();
    displayTargetPriorityText();
    displayTurretHintText();
    capsule.draw(game.getWindow());
    sf::CircleShape capsuleRangeCircle(capsule.getAttackRange());
    capsuleRangeCircle.setOrigin({capsule.getAttackRange(), capsule.getAttackRange()});
    capsuleRangeCircle.setPosition(capsule.getPosition());
    capsuleRangeCircle.setFillColor(sf::Color::Transparent);
    capsuleRangeCircle.setOutlineThickness(4.f);
    capsuleRangeCircle.setOutlineColor(sf::Color::Red);
    game.getWindow().draw(capsuleRangeCircle);
    spawner.draw(game.getWindow());
    if (turretPlaced) {
        sf::CircleShape turretRangeCircle(turret.getAttackRange());
        turretRangeCircle.setOrigin({turret.getAttackRange(), turret.getAttackRange()});
        turretRangeCircle.setPosition(turret.getPosition());
        turretRangeCircle.setFillColor(sf::Color::Transparent);
        turretRangeCircle.setOutlineThickness(3.f);
        turretRangeCircle.setOutlineColor(sf::Color(255, 140, 0));
        game.getWindow().draw(turretRangeCircle);
        turret.draw(game.getWindow());
    }

    for (auto& enemy : TargetList) {
        enemy.draw(game.getWindow());
    }
    for (auto& projectile : ProjectileList) {
        projectile.draw(game.getWindow());
    }

}

void PlayingState::displayHealthText()
{
    HealthText.setString("Health: " + std::to_string(static_cast<int>(capsule.getHealth())));
    drawTextWithWhiteBacking(game.getWindow(), HealthText);
}

void PlayingState::displayEnergyText()
{
    EnergyText.setString("Energy: " + std::to_string(static_cast<int>(capsule.getEnergy())));
    drawTextWithWhiteBacking(game.getWindow(), EnergyText);
}

void PlayingState::displayTargetPriorityText()
{
    const char* const label = (targetPriority == TargetPriority::Oldest) ? "Oldest (first spawned)"
                                                                           : "Newest (last spawned)";
    TargetPriorityText.setString(std::string("Target priority: ") + label + "  [P: toggle]");
    const auto b = TargetPriorityText.getLocalBounds();
    TargetPriorityText.setOrigin({b.size.x * 0.5f, b.size.y * 0.5f});
    TargetPriorityText.setPosition({
        game.getWindow().getSize().x * 0.5f,
        game.getWindow().getSize().y * 0.12f});
    drawTextWithWhiteBacking(game.getWindow(), TargetPriorityText);
}

void PlayingState::displayTurretHintText()
{
    if (!turretHintTriggered || turretPlaced) {
        return;
    }
    const auto b = TurretHintText.getLocalBounds();
    TurretHintText.setOrigin({b.size.x * 0.5f, b.size.y * 0.5f});
    TurretHintText.setPosition({
        game.getWindow().getSize().x * 0.5f,
        game.getWindow().getSize().y * 0.5f});
    drawTextWithWhiteBacking(game.getWindow(), TurretHintText);
}

void PlayingState::checkGameOver()
{
    if (capsule.getHealth() <= 0)
    {
        game.changeState(std::make_unique<LoseState>(game));
    }
}

void PlayingState::checkGameWon()
{
    if (capsule.getEnergy() >= 100)
    {
        game.changeState(std::make_unique<WinState>(game));
    }
}

/////// Enemy Collisions ///////
void PlayingState::resolveEnemyCollisions() {
    for (auto itA = TargetList.begin(); itA != TargetList.end(); ++itA) {
        if (!itA->isAlive()) {
            continue;
        }
        auto itB = itA;
        ++itB;
        for (; itB != TargetList.end(); ++itB) {
            if (!itB->isAlive()) {
                continue;
            }

            const sf::FloatRect a = itA->getGlobalBounds();
            const sf::FloatRect b = itB->getGlobalBounds();
            const auto intersection = a.findIntersection(b);
            if (!intersection.has_value()) {
                continue;
            }

            const float overlapX = intersection->size.x;
            const float overlapY = intersection->size.y;
            const sf::Vector2f cA = a.getCenter();
            const sf::Vector2f cB = b.getCenter();

            if (overlapX < overlapY) {
                const float half = overlapX * 0.5f;
                if (cA.x < cB.x) {
                    itA->translate({-half, 0.f});
                    itB->translate({half, 0.f});
                } else {
                    itA->translate({half, 0.f});
                    itB->translate({-half, 0.f});
                }
            } else {
                const float half = overlapY * 0.5f;
                if (cA.y < cB.y) {
                    itA->translate({0.f, -half});
                    itB->translate({0.f, half});
                } else {
                    itA->translate({0.f, half});
                    itB->translate({0.f, -half});
                }
            }
        }
    }
}

/////// Capsule Collisions ///////
void PlayingState::resolveEnemyCapsuleCollisions() {
    if (!capsule.isAlive()) {
        return;
    }
    const sf::FloatRect capBounds = capsule.getGlobalBounds();
    const sf::Vector2f capCenter = capBounds.getCenter();

    for (auto& enemy : TargetList) {
        if (!enemy.isAlive()) {
            continue;
        }
        const sf::FloatRect eBounds = enemy.getGlobalBounds();
        const auto intersection = eBounds.findIntersection(capBounds);
        if (!intersection.has_value()) {
            continue;
        }

        const float overlapX = intersection->size.x;
        const float overlapY = intersection->size.y;
        const sf::Vector2f enemyCenter = eBounds.getCenter();

        if (overlapX < overlapY) {
            const float pen = overlapX;
            if (enemyCenter.x < capCenter.x) {
                enemy.translate({-pen, 0.f});
            } else {
                enemy.translate({pen, 0.f});
            }
        } else {
            const float pen = overlapY;
            if (enemyCenter.y < capCenter.y) {
                enemy.translate({0.f, -pen});
            } else {
                enemy.translate({0.f, pen});
            }
        }
    }
}

namespace {
    constexpr int kEnemySeparationPasses = 3;
    }
    
    void PlayingState::resolveCollisions() {
        for (int pass = 0; pass < kEnemySeparationPasses; ++pass) {
            resolveEnemyCollisions();
            resolveEnemyCapsuleCollisions();
        }
    }

/////// Target Selection ///////
// Function to select a target from the target list
Enemy* PlayingState::selectTargetFromTargetList() const {
    // If the target priority is oldest, select the oldest enemy in the target list
    if (targetPriority == TargetPriority::Oldest) {
        for (auto& enemy : TargetList) {
            if (isEnemyInCapsuleRange(enemy)) {
                return const_cast<Enemy*>(&enemy);
            }
        }
        return nullptr;
    }
    // If the target priority is newest, select the newest enemy in the target list
    for (auto it = TargetList.rbegin(); it != TargetList.rend(); ++it) {
        if (isEnemyInCapsuleRange(*it)) {
            return const_cast<Enemy*>(&(*it));
        }
    }
    return nullptr;
}

// Function to check if an enemy is in range of the capsule
bool PlayingState::isEnemyInCapsuleRange(const Enemy& enemy) const {
    if (!enemy.isAlive()) {
        return false;
    }
    // Calculate the distance between the enemy and the capsule
    const sf::Vector2f d = enemy.getPosition() - capsule.getPosition();
    const float capRange = capsule.getAttackRange();
    const float capRangeSq = capRange * capRange;
    return (d.x * d.x + d.y * d.y) <= capRangeSq;
}

