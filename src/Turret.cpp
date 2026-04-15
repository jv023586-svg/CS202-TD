#include "Turret.h"

/*
    // variables 
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
*/

Turret::Turret() 
  : position({0.f, 0.f}),
    attackRange(0.f),
    damage(10.f),
    attackRate(2.f),
    attackCooldown(0.f),

    energy(100.f),
    maxEnergy(100.f),

    rechargeRate(15.f),

    health(100.f),
    maxHealth(100.f),

    target(nullptr)
{

}

Turret::Turret(float health, float maxHealth, float energy, float maxEnergy, 
               float rechargeRate, float attackCooldown, float attackRate, 
               float attackRange, float damage, sf::Vector2f position, 
               Enemy* target)
{
    // TODO: implement sprite 
}

void Turret::update(float dt) 
{
    // Update turret energy
    energy += rechargeRate * dt;
    if (energy > maxEnergy)
    {
        energy = maxEnergy;
    }

    // Update turret cooldown
    attackCooldown -= dt;
    if (attackCooldown < 0)
    {
        attackCooldown = 0;
    }

    // TODO: TargetQueue integration
}

void Turret::draw(sf::RenderWindow& window) 
{
    // window.draw(sprite);
}

void Turret::takeDamage(float amount)
{
    health -= amount;
    if (health < 0.f)
    {
        health = 0.f;
    }

}

bool Turret::isAlive()
{
    return health > 0.f;
}

bool Turret::isFullyCharged()
{
    return energy >= maxEnergy;
}

void Turret::acquireTarget(/* queue */)
{
    // TODO: implement enemy, attack, and target once we have queue
}

void Turret::attack()
{
    if (!target)
    {
        return;
    }

    // target->takeDamage(damage);

    // Reset cooldown 
   
    // Consume energy
}

