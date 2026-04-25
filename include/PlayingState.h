#pragma once
#include "State.h"
#include "Capsule.h"
#include "Enemy.h"
#include "Projectiles.h"
#include "Spawner.h"
#include "Turret.h"
#include "TargetPriority.h"
#include <list>
#include <vector>

class PlayingState : public State {
public:
    explicit PlayingState(Game& game);

    void onEnter() override;
    void onExit() override;

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    BackgroundId getBackgroundId() const override;

private:
    Capsule capsule;
    Spawner spawner;
    Turret turret;
    sf::Text HealthText;
    sf::Text EnergyText;
    sf::Text TargetPriorityText;
    sf::Text TurretHintText;
    std::list<Enemy> TargetList;
    std::list<Projectile> ProjectileList;
    
    int nextSpawnIndex{0};
    TargetPriority targetPriority{TargetPriority::Oldest};
    bool turretHintTriggered{false};
    bool turretPlaced{false};

    float stateTimer;

    void checkGameOver();
    void checkGameWon();

    void displayHealthText();
    void displayEnergyText();
    void displayTargetPriorityText();
    void displayTurretHintText();
    void resolveEnemyCollisions();
    void resolveEnemyCapsuleCollisions();
    void resolveCollisions();
    Enemy* selectTargetFromTargetList() const;
    bool isEnemyInCapsuleRange(const Enemy& enemy) const;

};