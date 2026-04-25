#include "Capsule.h"
#include <climits>
#include <iostream>
#include <random>

/////// Constructors ///////
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

/////// Sync Functions ///////
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
    setCapsuleVisualScale(visualScale);
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

// Function to draw the Capsule to the screen
void Capsule::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(capsuleHitbox);
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

    if (priority == TargetPriority::Oldest) {
        attackTarget = alive.front();
        return;
    }
    attackTarget = alive.back();
}

/////// Getters ///////

float Capsule::getHealth() const {
    return health;
}
float Capsule::getMaxHealth() const {
    return maxHealth;
}
float Capsule::getEnergy() const {
    return energy;
}
float Capsule::getMaxEnergy() const {
    return maxEnergy;
}
float Capsule::getRechargeRate() const {
    return rechargeRate;
}
float Capsule::getAttackRange() const {
    return attackRange;
}
float Capsule::getAttackRate() const {
    return attackRate;
}
float Capsule::getAttackCooldown() const {
    return attackCooldown;
}
float Capsule::getAttackDamage() const {
    return damage;
}
sf::Vector2f Capsule::getPosition() const {
    return position;
}
sf::FloatRect Capsule::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

/////// Attack Functions ///////
// Function to get the attack target
Enemy* Capsule::getAttackTarget() const {
    return attackTarget;
}
// Function to set the attack target directly
void Capsule::setAttackTarget(Enemy* target) {
    attackTarget = target;
}
// Function to clear the attack target
void Capsule::clearAttackTarget() {
    attackTarget = nullptr;
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

void Capsule::takeDamage(float damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

/////// Setters ///////

void Capsule::setCapsuleHealth(float health) {
    this->health = health;
}
void Capsule::setCapsuleMaxHealth(float maxHealth) {
    this->maxHealth = maxHealth;
}
void Capsule::setCapsuleEnergy(float energy) {
    this->energy = energy;
}
void Capsule::setCapsuleMaxEnergy(float maxEnergy) {
    this->maxEnergy = maxEnergy;
}
void Capsule::setCapsuleRechargeRate(float rechargeRate) {
    this->rechargeRate = rechargeRate;
}
void Capsule::setCapsuleAttackCooldown(float attackCooldown) {
    this->attackCooldown = attackCooldown;
}
void Capsule::setCapsuleAttackRate(float attackRate) {
    this->attackRate = attackRate;
}
void Capsule::setCapsuleAttackRange(float attackRange) {
    this->attackRange = attackRange;
}
void Capsule::setCapsuleAttackDamage(float damage) {
    this->damage = damage;
}
void Capsule::setCapsulePosition(const sf::Vector2f& position) {
    this->position = position;
    sprite.setPosition(position);
    capsuleHitbox.setPosition(position);
}
void Capsule::setCapsuleVisualScale(const sf::Vector2f& visualScale) {
    this->visualScale = visualScale;
    sprite.setScale(visualScale);
    capsuleHitbox.setScale(visualScale);
}

///////Other Functions ///////
bool Capsule::isAlive() const {
    return health > 0;
}
bool Capsule::isFullyCharged() const {
    return energy >= maxEnergy;
}
