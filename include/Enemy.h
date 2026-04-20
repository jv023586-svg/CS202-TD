#pragma once
#include <SFML/Graphics.hpp>

class Capsule;

class Enemy {
public:
    Enemy(); // Constructor
    Enemy(float health, float maxHealth, float damage, float attackRate, float attackCooldown, float speed, sf::Vector2f position, sf::Vector2f moveGoal, int spawnIndex = -1);

    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;
    Enemy(Enemy&& other) noexcept;
    Enemy& operator=(Enemy&& other) noexcept;

    void update(float dt); // Updates the position of the enemy
    void draw(sf::RenderWindow& window); // Draws the enemy to the screen
    void setPosition(const sf::Vector2f& newPosition); // Sets the position of the enemy
    void setMoveGoal(const sf::Vector2f& goal);
    void setSpawnIndex(int index);
    int getSpawnIndex() const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;
    void translate(const sf::Vector2f& delta);
    /// Single source of truth: scales both the sprite and the debug hitbox together.
    void setVisualScale(const sf::Vector2f& newScale);
    void takeDamage(int amount); // Takes damage from the enemy
    bool isAlive() const; // Checks if the enemy is alive
    /// Melee hits when enemy sprite bounds overlap the capsule (same as hull contact).
    void tryMeleeAttack(Capsule& capsule);

private:
    void syncDrawablesAfterTextureLoad(); // Sets origins, hitbox size, applies visualScale

    float health;
    float maxHealth;
    float damage;
    float attackRate;
    float attackCooldown;
    float speed;

    sf::Vector2f position;
    sf::Vector2f moveGoal;
    int spawnIndex;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RectangleShape enemyHitbox; // Debug AABB aligned to scaled sprite (same visualScale)
    sf::Vector2f visualScale{2.0f, 2.0f}; // Applied to both sprite and hitbox
};