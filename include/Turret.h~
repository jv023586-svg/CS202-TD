#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.h"

class Turret {
    public:
        Turret();

        void update(float dt);
        void draw(sf::RenderWindow& window);
        void takeDamage(float amount);
        bool isAlive();
        bool isFullyCharged();
        void acquireTarget(queue);
        void attack();
 
    private:
        sf::Vector2f position;
        float attackRange;
        float damage;
        float attackRate;
        float attackCooldown;
        float energy;
        float maxEnergy;
        float rechargeRate;
        float health;
        float maxHealth;
        Enemy* target;
};
