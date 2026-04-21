#include "Capsule.h"
#include <climits>
#include <iostream>
#include <random>

// Constructor with default parameters
Capsule::Capsule()
    : health(1000.f), // current health of the capsule
      maxHealth(1000.f), // maximum health of the capsule
      energy(1.0f), // current energy of the capsule
      maxEnergy(100.f), // maximum energy of the capsule
      rechargeRate(1.0f), // rate at which the capsule recharges energy
      attackCooldown(3.f), // time until the capsule can attack again
      attackRate(1.f), // rate at which the capsule attacks
      attackRange(500.f), // 500 pixels range of the capsule's attack
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

// Constructor with parameters
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

// Function to sync the drawables after the texture is loaded
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

// Function to update the capsule
void Capsule::update(float dt) {
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

// This function is used to resolve the attack target for the capsule.
// It takes a list of enemies in range and a priority.
// It returns the enemy that the capsule should attack.
// The priority is used to determine which enemy to attack.
// The priority can be either random or first.
// If the priority is random, the capsule will attack a random enemy in range.
// If the priority is first, the capsule will attack the first enemy in range.
// The whole point of this function is to change the pointer that attackTarget points to.
void Capsule::resolveAttackTarget(const std::vector<Enemy*>& inRange, TargetPriority priority) {
    std::vector<Enemy*> alive; // list of enemies in range
    alive.reserve(inRange.size()); // reserve space for the list of enemies in range
    for (Enemy* e : inRange) {  // for each enemy in range
        if (e && e->isAlive()) { // if the enemy is alive
            alive.push_back(e); // add the enemy to the list of alive enemies
        }
    }
    if (alive.empty()) { // if there are no alive enemies in range
        attackTarget = nullptr; // set the attack target to nullptr
        return; // return and do nothing
    }

    if (priority == TargetPriority::First) { // if the priority is first
        Enemy* best = alive[0]; // set the best enemy to the first enemy in the list of alive enemies
        int bestKey = best->getSpawnIndex(); // get the spawn index of the best enemy
        if (bestKey < 0) { // if the spawn index is less than 0
            bestKey = INT_MAX; // set the best key to the maximum integer value
        }
        for (std::size_t i = 1; i < alive.size(); ++i) { // for each enemy in the list of alive enemies
            Enemy* e = alive[i]; // set the enemy to the current enemy in the list of alive enemies
            int k = e->getSpawnIndex(); // get the spawn index of the current enemy
            if (k < 0) { // if the spawn index is less than 0
                k = INT_MAX; // set the spawn index to the maximum integer value
            }
            if (k < bestKey) { // if the spawn index of the current enemy is less than the spawn index of the best enemy
                bestKey = k; // set the best key to the spawn index of the current enemy
                best = e; // set the best enemy to the current enemy
            }
        }
        attackTarget = best; // set the attack target to the best enemy
        return; // return and do nothing
    }

    auto stillIn = [&](Enemy* t) { // lambda function to check if the enemy is in the list of alive enemies
        for (Enemy* e : alive) { // for each enemy in the list of alive enemies
            if (e == t) {
                return true; // return true if the enemy is in the list of alive enemies
            }
        }
        return false; // return false if the enemy is not in the list of alive enemies
    };
    if (attackTarget && attackTarget->isAlive() && stillIn(attackTarget)) { // if the attack target is not nullptr and the attack target is alive and the attack target is in the list of alive enemies
        return; // return and do nothing
    }

    static thread_local std::mt19937 rng(std::random_device{}()); // create a random number generator
    std::uniform_int_distribution<std::size_t> dist(0, alive.size() - 1); // create a uniform distribution for the size of the list of alive enemies
    attackTarget = alive[dist(rng)]; // set the attack target to a random enemy in the list of alive enemies
}

// Function to get the attack target
Enemy* Capsule::getAttackTarget() const {
    return attackTarget;
}

// Function to clear the attack target
void Capsule::clearAttackTarget() {
    attackTarget = nullptr;
}

// Function to get the position of the Capsule
sf::Vector2f Capsule::getPosition() const {
    return position;
}

// Function to get the global bounds of the Capsule
sf::FloatRect Capsule::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

// Function to get the attack range of the Capsule
float Capsule::getAttackRange() const {
    return attackRange;
}

// Function to draw the Capsule to the screen
void Capsule::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(capsuleHitbox);
}

// Function to set the position of the Capsule
void Capsule::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    sprite.setPosition(position);
    capsuleHitbox.setPosition(position);
}

// Function to set the visual scale of the Capsule
void Capsule::setVisualScale(const sf::Vector2f& newScale) {
    visualScale = newScale;
    sprite.setScale(visualScale);
    capsuleHitbox.setScale(visualScale);
}

// Function to check if the Capsule is alive
bool Capsule::isAlive() const {
    return health > 0;
}

// Function to get the health of the Capsule
float Capsule::getHealth() const {
    return health;
}

// Function to get the energy of the Capsule
float Capsule::getEnergy() const {
    return energy;
}

// Function to attack the target
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

// Function to check if the Capsule is fully charged
bool Capsule::isFullyCharged() const {
    return energy >= maxEnergy;
}

// Function to take damage from an enemy
void Capsule::takeDamage(float damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

// Function to set the energy of the Capsule
void Capsule::setCapsuleEnergy(float energy) {
    this->energy = energy;
}

// Function to set the health of the Capsule
void Capsule::setCapsuleHealth(float health) {
    this->health = health;
}