#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Enemy.h"
#include "TargetPriority.h"

class Capsule {
public:
    Capsule(); // Constructor
    Capsule(float health, float maxHealth, float energy, float maxEnergy, float rechargeRate, float attackCooldown, float attackRate, float attackRange, float damage, sf::Vector2f position, Enemy* initialAttackTarget); // Constructor with parameters

    void update(float dt); // Updates the position of the capsule
    void draw(sf::RenderWindow& window); // Draws the capsule to the screen

    
    
    /////// Attack Functions ///////
    bool attack();
    void resolveAttackTarget(const std::vector<Enemy*>& inRange, TargetPriority priority);
    Enemy* getAttackTarget() const;
    void clearAttackTarget();
    void takeDamage(float damage); // Takes damage from an enemy
    
    /////// Getters ///////
    float getHealth() const;
    float getMaxHealth() const;
    float getEnergy() const;
    float getMaxEnergy() const;
    float getRechargeRate() const;
    float getAttackRange() const;
    float getAttackCooldown() const;
    float getAttackRate() const;
    float getAttackDamage() const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;

    /////// Setters ///////
    void setCapsuleHealth(float health);
    void setCapsuleMaxHealth(float maxHealth);
    void setCapsuleEnergy(float energy);
    void setCapsuleMaxEnergy(float maxEnergy);
    void setCapsuleRechargeRate(float rechargeRate);
    void setCapsuleAttackCooldown(float attackCooldown);
    void setCapsuleAttackRate(float attackRate);
    void setCapsuleAttackRange(float attackRange);
    void setCapsuleAttackDamage(float attackDamage);
    void setCapsulePosition(const sf::Vector2f& position);
    void setCapsuleVisualScale(const sf::Vector2f& visualScale);
    
    /////// Other Functions ///////
    bool isAlive() const; // Checks if the capsule is alive 
    bool isFullyCharged() const; // Checks if the capsule is fully charged

private:
    void syncDrawablesAfterTextureLoad(); // Sets origins, hitbox size, applies visualScale

    float health; // Current health of the capsule
    float maxHealth; // Maximum health of the capsule
    float energy; // Current energy of the capsule
    float maxEnergy; // Maximum energy of the capsule
    float rechargeRate; // Rate at which the capsule recharges energy
    float attackCooldown; // Time until the capsule can attack again
    float attackRate; // Rate at which the capsule attacks
    float attackRange; // Range of the capsule's attack
    float damage; // Damage of the capsule's attack

    sf::Vector2f position; // Position of the capsule
    Enemy* attackTarget; // Current ranged attack target (set by resolveAttackTarget)
    sf::Texture texture; // Capsule texture
    sf::Sprite sprite; // Capsule sprite
    sf::RectangleShape capsuleHitbox; // Debug AABB aligned to scaled sprite (same visualScale)
    sf::Vector2f visualScale{0.3f, 0.3f}; // Applied to both sprite and hitbox
};