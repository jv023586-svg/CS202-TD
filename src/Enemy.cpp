#include "Enemy.h"

using namespace std;

Enemy::Enemy(float health(100.0f), float maxHealth(100.0f), float damage(10.0f), float attackRate(1.0f), 
             float attackCooldown(1.0f), float speed(1.0f), const sf::Vector2f& position(0.0f, 0.0f), 
             const sf::Vector2f& target(0.0f, 0.0f)) 
{

    if (!texture.loadFromFile("assets/textures/bloop.png"))
    {
        std::cerr << "Failed to load enemy texture assets/textures/bloop.png" 
                  << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setScale(0.5f, 0.5f);    
}

void Enemy::update(float dt) 
{
    
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