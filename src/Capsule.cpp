#include "Capsule.h"
#include <climits>
#include <iostream>
#include <random>

Capsule::Capsule()
    : health(1000.f), // current health of the capsule
      maxHealth(1000.f), // maximum health of the capsule
      energy(1.0f), // current energy of the capsule
      maxEnergy(100.f), // maximum energy of the capsule
      rechargeRate(1.0f), // rate at which the capsule recharges energy
      attackCooldown(3.f), // time until the capsule can attack again
      attackRate(1.f), // rate at which the capsule attacks
      attackRange(500.f), // 120 pixels range of the capsule's attack
      damage(35.0f), // damage of the capsule's attack
      position({200.f, 750.f}), // position of the capsule
      attackTarget(nullptr),
      texture(),
      sprite(texture) 
    {
    // Load the capsule texture from the file
    if (!texture.loadFromFile("assets/textures/capsule.png")) {
        std::cerr << "Failed to load capsule texture: assets/textures/capsule.png\n";
    } else {
        syncDrawablesAfterTextureLoad(); // sets origins, hitbox size, applies visualScale
    }
}

void Capsule::syncDrawablesAfterTextureLoad() {
    sprite.setTexture(texture, true);
    const auto bounds = sprite.getLocalBounds();
    const float hx = bounds.size.x * 0.5f;
    const float hy = bounds.size.y * 0.5f;
    sprite.setOrigin({hx, hy});
    sprite.setPosition(position);
    capsuleHitbox.setSize({bounds.size.x, bounds.size.y});
    capsuleHitbox.setOrigin({hx, hy});
    capsuleHitbox.setPosition(position);
    capsuleHitbox.setFillColor(sf::Color::Transparent);
    capsuleHitbox.setOutlineColor(sf::Color::Red);
    capsuleHitbox.setOutlineThickness(10.f);
    setVisualScale(visualScale);
}

void Capsule::update(float dt) { // Updates the position of the capsule
    // Update the energy of the capsule
    energy += rechargeRate * dt;
    if (energy > maxEnergy) {
        energy = maxEnergy;
    }
    // Update the attack cooldown of the capsule
    attackCooldown -= dt;
    if (attackCooldown < 0) {
        attackCooldown = 0;
    }
}
// I don't love this. I want to break this down so I understand.
// This function is used to resolve the attack target for the capsule.
// It takes a list of enemies in range and a priority.
// It returns the enemy that the capsule should attack.
// The priority is used to determine which enemy to attack.
// The priority can be either random or first.
// If the priority is random, the capsule will attack a random enemy in range.
// If the priority is first, the capsule will attack the first enemy in range.
void Capsule::resolveAttackTarget(const std::vector<Enemy*>& inRange, TargetPriority priority) {
    std::vector<Enemy*> alive;
    alive.reserve(inRange.size());
    for (Enemy* e : inRange) {
        if (e && e->isAlive()) {
            alive.push_back(e);
        }
    }
    if (alive.empty()) {
        attackTarget = nullptr;
        return;
    }

    if (priority == TargetPriority::First) {
        Enemy* best = alive[0];
        int bestKey = best->getSpawnIndex();
        if (bestKey < 0) {
            bestKey = INT_MAX;
        }
        for (std::size_t i = 1; i < alive.size(); ++i) {
            Enemy* e = alive[i];
            int k = e->getSpawnIndex();
            if (k < 0) {
                k = INT_MAX;
            }
            if (k < bestKey) {
                bestKey = k;
                best = e;
            }
        }
        attackTarget = best;
        return;
    }

    auto stillIn = [&](Enemy* t) {
        for (Enemy* e : alive) {
            if (e == t) {
                return true;
            }
        }
        return false;
    };
    if (attackTarget && attackTarget->isAlive() && stillIn(attackTarget)) {
        return;
    }

    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, alive.size() - 1);
    attackTarget = alive[dist(rng)];
}

Enemy* Capsule::getAttackTarget() const {
    return attackTarget;
}

void Capsule::clearAttackTarget() {
    attackTarget = nullptr;
}

sf::Vector2f Capsule::getPosition() const {
    return position;
}

sf::FloatRect Capsule::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

float Capsule::getAttackRange() const {
    return attackRange;
}

Capsule::Capsule(float health, float maxHealth, float energy, float maxEnergy, float rechargeRate, float attackCooldown, float attackRate, float attackRange, float damage, sf::Vector2f position, Enemy* initialAttackTarget)
    : texture(),
      sprite(texture) {
    this->health = health;
    this->maxHealth = maxHealth;
    this->energy = energy;
    this->maxEnergy = maxEnergy;
    this->rechargeRate = rechargeRate;
    this->attackCooldown = attackCooldown;
    this->attackRate = attackRate;
    this->attackRange = attackRange;
    this->damage = damage;
    this->position = position;
    this->attackTarget = initialAttackTarget;

    if (!texture.loadFromFile("assets/textures/capsule.png")) {
        std::cerr << "Failed to load capsule texture: assets/textures/capsule.png\n";
    } else {
        syncDrawablesAfterTextureLoad();
    }
}

void Capsule::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(capsuleHitbox);
}

void Capsule::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    sprite.setPosition(position);
    capsuleHitbox.setPosition(position);
}

void Capsule::setVisualScale(const sf::Vector2f& newScale) {
    visualScale = newScale;
    sprite.setScale(visualScale);
    capsuleHitbox.setScale(visualScale);
}

bool Capsule::isAlive() const {
    return health > 0;
}

float Capsule::getHealth() const {
    return health;
}

float Capsule::getEnergy() const {
    return energy;
}

bool Capsule::attack() {
    if (attackCooldown > 0) {
        return false;
    }
    if (!attackTarget) {
        return false;
    }
    if (!attackTarget->isAlive()) {
        return false;
    }
    attackTarget->takeDamage(static_cast<int>(damage));
    attackCooldown = attackRate;
    return true;
}

bool Capsule::isFullyCharged() const {
    return energy >= maxEnergy;
}

void Capsule::takeDamage(float damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

void Capsule::setCapsuleEnergy(float energy) {
    this->energy = energy;
}
