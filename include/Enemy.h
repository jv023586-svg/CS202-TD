#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy();

    void update();
    void draw(sf::RenderWindow& window);

    void takeDamage(int amount);
    bool isAlive() const;
    void attack();

private:
    float health;
    float maxHealth;
    float damage;
    float attackRate;
    float attackCooldown;
    float speed;

    sf::Vector2f position;
    sf::Vector2f target;
};