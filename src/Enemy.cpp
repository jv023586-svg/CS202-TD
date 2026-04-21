#include "Enemy.h"
#include "Capsule.h"
#include <cmath>
#include <iostream>


// SFML rects treat some edge-touching as non-overlapping; allow melee when gap between AABBs is within this (pixels).
constexpr float kMeleeContactSlopPx = 8.f;

// Constructor with default parameters
Enemy::Enemy()
    : health(100.0f),
      maxHealth(100.0f),
      attackCooldown(0.0f),
      attackRate(1.0f),
      damage(10.0f),
      position(0.0f, 0.0f),
      moveGoal(0.0f, 0.0f),
      spawnIndex(-1),
      speed(1.0f),
      texture(),
      sprite(texture)
    {
    // Load the enemy texture from the file
    if (!texture.loadFromFile("assets/textures/enemy.png")) {
        std::cerr << "Failed to load enemy texture assets/textures/enemy.png\n";
    } else {
        syncDrawablesAfterTextureLoad(); // sets origins, hitbox size, applies visualScale
    }
}
// Constructor with parameters
Enemy::Enemy(float health, float maxHealth, float damage, float attackRate, float attackCooldown, float speed, sf::Vector2f position, sf::Vector2f moveGoal, int spawnIndex)
    : health(health),
      maxHealth(maxHealth),
      damage(damage),
      attackRate(attackRate),
      attackCooldown(attackCooldown),
      speed(speed),
      position(position),
      moveGoal(moveGoal),
      spawnIndex(spawnIndex),
      texture(),
      sprite(texture) {
    if (!texture.loadFromFile("assets/textures/enemy.png")) {
        std::cerr << "Failed to load enemy texture assets/textures/enemy.png\n";
    } else {
        syncDrawablesAfterTextureLoad();
    }
}

// Move constructor
Enemy::Enemy(Enemy&& other) noexcept
    : health(other.health),
      maxHealth(other.maxHealth),
      damage(other.damage),
      attackRate(other.attackRate),
      attackCooldown(other.attackCooldown),
      speed(other.speed),
      position(other.position),
      moveGoal(other.moveGoal),
      spawnIndex(other.spawnIndex),
      texture(std::move(other.texture)),
      sprite(std::move(other.sprite)),
      enemyHitbox(std::move(other.enemyHitbox)),
      visualScale(other.visualScale) {
    // Re-bind after move; false keeps rect/origin from the moved sprite.
    sprite.setTexture(texture, false);
    sprite.setPosition(position);
    enemyHitbox.setPosition(position);
    setVisualScale(visualScale);
}


namespace {
// Function to calculate the squared gap between two non-overlapping axis-aligned rects (0 if they overlap on both axes).
float separationDistanceSquared(const sf::FloatRect& a, const sf::FloatRect& b) {
    float dx = 0.f;
    if (a.position.x + a.size.x <= b.position.x) {
        dx = b.position.x - (a.position.x + a.size.x);
    } else if (b.position.x + b.size.x <= a.position.x) {
        dx = a.position.x - (b.position.x + b.size.x);
    }
    float dy = 0.f;
    if (a.position.y + a.size.y <= b.position.y) {
        dy = b.position.y - (a.position.y + a.size.y);
    } else if (b.position.y + b.size.y <= a.position.y) {
        dy = a.position.y - (b.position.y + b.size.y);
    }
    return dx * dx + dy * dy;
}

// Function to check if the enemy can melee the capsule
bool canMeleeCapsule(const sf::FloatRect& enemyBounds, const sf::FloatRect& capBounds) {
    if (enemyBounds.findIntersection(capBounds).has_value()) {
        return true;
    }
    return separationDistanceSquared(enemyBounds, capBounds) <= kMeleeContactSlopPx * kMeleeContactSlopPx;
}
} // namespace

// Move assignment operator
Enemy& Enemy::operator=(Enemy&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    health = other.health;
    maxHealth = other.maxHealth;
    damage = other.damage;
    attackRate = other.attackRate;
    attackCooldown = other.attackCooldown;
    speed = other.speed;
    position = other.position;
    moveGoal = other.moveGoal;
    spawnIndex = other.spawnIndex;
    texture = std::move(other.texture);
    sprite = std::move(other.sprite);
    enemyHitbox = std::move(other.enemyHitbox);
    visualScale = other.visualScale;
    sprite.setTexture(texture, false);
    sprite.setPosition(position);
    enemyHitbox.setPosition(position);
    setVisualScale(visualScale);
    return *this;
}

// Function to sync the drawables after the texture is loaded
void Enemy::syncDrawablesAfterTextureLoad() {
    sprite.setTexture(texture, true);
    const auto bounds = sprite.getLocalBounds();
    const float hx = bounds.size.x * 0.5f;
    const float hy = bounds.size.y * 0.5f;
    sprite.setOrigin({hx, hy});
    sprite.setPosition(position);
    enemyHitbox.setSize({bounds.size.x, bounds.size.y});
    enemyHitbox.setOrigin({hx, hy});
    enemyHitbox.setPosition(position);
    enemyHitbox.setFillColor(sf::Color::Transparent);
    enemyHitbox.setOutlineColor(sf::Color::Red);
    enemyHitbox.setOutlineThickness(1.0f);
    setVisualScale(visualScale);
}

// Function to set the position of the enemy
void Enemy::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    sprite.setPosition(position);
    enemyHitbox.setPosition(position);
}

// Function to update the enemy position in sprite as it moves towards capsule
void Enemy::update(float dt)
{
    attackCooldown -= dt;
    if (attackCooldown < 0.f) {
        attackCooldown = 0.f;
    }

    // Creates a vector pointing from enemy to capsule
    sf::Vector2f dir = moveGoal - position;
    
    // Finds distance to target, vector magnitude
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.0001f) {
        dir /= len;
        // Moves enemy in direction, constant velocity
        position += dir * speed * dt;
        sprite.setPosition(position);
        enemyHitbox.setPosition(position);
    }
}

// Function to draw the enemy to the screen
void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    window.draw(enemyHitbox);
}

// Function to take damage
void Enemy::takeDamage(int amount)
{
    health -= amount;
    if (health <= 0)
    {
        health = 0;
    }
}

// Function to check if the enemy is alive
bool Enemy::isAlive() const
{
    return health > 0;
}

// Function to try to melee attack the capsule
void Enemy::tryMeleeAttack(Capsule& capsule) {
    if (attackCooldown > 0.f) {
        return;
    }
    if (!capsule.isAlive()) {
        return;
    }
    const sf::FloatRect enemyBounds = getGlobalBounds();
    const sf::FloatRect capBounds = capsule.getGlobalBounds();
    if (!canMeleeCapsule(enemyBounds, capBounds)) {
        return;
    }
    capsule.takeDamage(damage);
    attackCooldown = attackRate;
}

// Function to set the movement destination
void Enemy::setMoveGoal(const sf::Vector2f& goal) {
    moveGoal = goal;
}

// Function to set the spawn index
void Enemy::setSpawnIndex(int index) {
    spawnIndex = index;
}

// Function to get the spawn index
int Enemy::getSpawnIndex() const {
    return spawnIndex;
}

// Function to get the position
sf::Vector2f Enemy::getPosition() const {
    return position;
}

// Function to get the global bounds
sf::FloatRect Enemy::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

// Function to translate (move)
void Enemy::translate(const sf::Vector2f& delta) {
    setPosition(position + delta);
}

// Function to set the visual scale
void Enemy::setVisualScale(const sf::Vector2f& newScale) {
    visualScale = newScale;
    sprite.setScale(visualScale);
    enemyHitbox.setScale(visualScale);
}