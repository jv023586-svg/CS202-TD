#include "Enemy.h"
#include <iostream>
#include <cmath>

Enemy::Enemy()
    : health(100.0f),
      maxHealth(100.0f),
      damage(10.0f),
      attackRate(1.0f),
      attackCooldown(1.0f),
      speed(1.0f),
      position(0.0f, 0.0f),
      target(0.0f, 0.0f),
      sprite(texture)
{
    if (!texture.loadFromFile("assets/textures/enemy.png")) {
        std::cerr << "Failed to load enemy texture assets/textures/enemy.png\n";
    }

    sprite.setTexture(texture, true);

    // Center origin to visual center
    const auto b = sprite.getLocalBounds();
    sprite.setOrigin({b.size.x * 0.5f, b.size.y * 0.5f});

    // 1920x1080p game window, center of the screen is 960x540
    position = {960.f, 540.f};
    sprite.setPosition(position);

    // Massive scale to see it appear on screen currently 
    // Fix later TODO
    sprite.setScale({8.0f, 8.0f});
}

// Update the enemy position in sprite as it moves towards capsule
void Enemy::update(float dt)
{
    // Creates a vector pointing from enemy to capsule
    sf::Vector2f dir = target - position;
    
    // Finds distance to target, vector magnitude
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.0001f) {
        dir /= len;
        // Moves enemy in direction, constant velocity
        position += dir * speed * dt;
        // Sync sprite to position
        sprite.setPosition(position);
    }
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Enemy::takeDamage(int amount)
{
    health -= amount;
    if (health <= 0)
    {
        health = 0;
    }
}

bool Enemy::isAlive() const
{
    return health > 0;
}

void Enemy::attack()
{
}
