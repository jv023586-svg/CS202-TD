#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.h"

class Turret {
public:
    Turret();
    Turret(float health, float maxHealth, float energy, float maxEnergy, float rechargeRate, float attackCooldown,
           float attackRate, float attackRange, float damage, sf::Vector2f position, Enemy* attackTarget);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    void takeDamage(float amount);
    bool isAlive();
    bool isFullyCharged();
    void setAttackTarget(Enemy* enemy);
    void setPosition(const sf::Vector2f& p);
    void setTurretVisualScale(const sf::Vector2f& scale);
    sf::Vector2f getPosition() const;
    float getAttackRange() const;
    Enemy* getAttackTarget() const;
    /// Returns true if a shot was fired (damage applied).
    bool attack();

private:
    void syncDrawable();

    sf::Vector2f position;
    sf::Vector2f visualScale{1.f, 1.f};
    float attackRange;
    float damage;
    float attackRate;
    float attackCooldown;
    float energy;
    float maxEnergy;
    float rechargeRate;
    float health;
    float maxHealth;
    Enemy* attackTarget;
    sf::Texture texture;
    sf::Sprite sprite;
};
