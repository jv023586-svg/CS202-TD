#include "Turret.h"

Turret::Turret()
    : position({900.f, 750.f}),
      attackRange(75.f),
      damage(10.f),
      attackRate(1.2f),
      attackCooldown(0.f),
      energy(100.f),
      maxEnergy(100.f),
      rechargeRate(15.f),
      health(100.f),
      maxHealth(100.f),
      attackTarget(nullptr) {}

Turret::Turret(float health, float maxHealth, float energy, float maxEnergy, float rechargeRate, float attackCooldown,
               float attackRate, float attackRange, float damage, sf::Vector2f position, Enemy* attackTarget)
    : position(position),
      attackRange(attackRange),
      damage(damage),
      attackRate(attackRate),
      attackCooldown(attackCooldown),
      energy(energy),
      maxEnergy(maxEnergy),
      rechargeRate(rechargeRate),
      health(health),
      maxHealth(maxHealth),
      attackTarget(attackTarget) {}

void Turret::update(float dt) {
    energy += rechargeRate * dt;
    if (energy > maxEnergy) {
        energy = maxEnergy;
    }

    attackCooldown -= dt;
    if (attackCooldown < 0.f) {
        attackCooldown = 0.f;
    }
}

void Turret::draw(sf::RenderWindow&) {}

void Turret::takeDamage(float amount) {
    health -= amount;
    if (health < 0.f) {
        health = 0.f;
    }
}

bool Turret::isAlive() {
    return health > 0.f;
}

bool Turret::isFullyCharged() {
    return energy >= maxEnergy;
}

void Turret::setAttackTarget(Enemy* enemy) {
    attackTarget = enemy;
}

void Turret::setPosition(const sf::Vector2f& p) {
    position = p;
}

sf::Vector2f Turret::getPosition() const {
    return position;
}

float Turret::getAttackRange() const {
    return attackRange;
}

Enemy* Turret::getAttackTarget() const {
    return attackTarget;
}

bool Turret::attack() {
    if (attackCooldown > 0.f) {
        return false;
    }
    if (!attackTarget || !attackTarget->isAlive()) {
        return false;
    }
    const sf::Vector2f d = attackTarget->getPosition() - position;
    const float r2 = attackRange * attackRange;
    if ((d.x * d.x + d.y * d.y) > r2) {
        return false;
    }
    attackTarget->takeDamage(static_cast<int>(damage));
    attackCooldown = attackRate;
    return true;
}
