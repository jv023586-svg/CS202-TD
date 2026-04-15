#include "Enemy.h"
#include <iostream>

Enemy::Enemy()
    : health(100.0f),
      maxHealth(100.0f),
      damage(10.0f),
      attackRate(1.0f),
      attackCooldown(1.0f),
      speed(1.0f),
      position(0.0f, 0.0f),
      target(0.0f, 0.0f)
{
    if (!texture.loadFromFile("assets/textures/bloop.png"))
    {
        std::cerr << "Failed to load enemy texture assets/textures/bloop.png\n";
    }

    // Sets scale, texture, start position of enemy
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setScale({0.5f, 0.5f});
}

void Enemy::update(float dt)
{
    // temporary test movement
    position.x += speed * dt * 60.0f;
    sprite.setPosition(position);
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