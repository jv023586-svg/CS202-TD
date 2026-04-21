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
    void setPosition(const sf::Vector2f& newPosition); // Sets position and sprite position
    /// Single source of truth: scales both the sprite and the debug hitbox together.
    void setVisualScale(const sf::Vector2f& newScale);
    void takeDamage(float damage); // Takes damage from an enemy
    bool isFullyCharged() const; // Checks if the capsule is fully charged
    bool isAlive() const; // Checks if the capsule is alive
    float getHealth() const;
    float getEnergy() const;
    /// Returns true if a shot was fired (damage applied). Used to spawn VFX only.
    bool attack();
    void setCapsuleEnergy(float energy);
    void setCapsuleHealth(float health);

    /// Call each frame after building the list of enemies currently in attack range.
    void resolveAttackTarget(const std::vector<Enemy*>& inRange, TargetPriority priority);
    Enemy* getAttackTarget() const;
    /// Call before removing dead enemies from the world if the current target may be destroyed.
    void clearAttackTarget();
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;
    float getAttackRange() const;

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