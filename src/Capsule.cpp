#include "Capsule.h"
#include <iostream>

Capsule::Capsule()
    : health(100.f), // current health of the capsule
      maxHealth(100.f), // maximum health of the capsule
      energy(100.f), // current energy of the capsule
      maxEnergy(100.f), // maximum energy of the capsule
      rechargeRate(15.f), // rate at which the capsule recharges energy
      attackCooldown(3.f), // time until the capsule can attack again
      attackRate(1.f), // rate at which the capsule attacks
      attackRange(120.f), // 120 pixels range of the capsule's attack
      damage(10.f), // damage of the capsule's attack
      position({200.f, 750.f}), // position of the capsule
      target(nullptr), // target of the capsule's attack
      texture(),
      sprite(texture) {
    if (!texture.loadFromFile("assets/textures/capsule.png")) {
        std::cerr << "Failed to load capsule texture: assets/textures/capsule.png\n";
    } else {
        sprite.setTexture(texture, true);
        const auto bounds = sprite.getLocalBounds(); // Get the local bounds of the capsule sprite
        sprite.setOrigin({bounds.size.x * 0.5f, bounds.size.y * 0.5f}); // Set the origin of the capsule sprite to the center of the sprite
        sprite.setPosition(position); // Set the position of the capsule sprite to the position of the capsule
        sprite.setScale({0.3f, 0.3f}); // Set the scale of the capsule sprite to 0.3f
        capsuleRectangle = sf::RectangleShape(sf::Vector2f(bounds.size.x, bounds.size.y)); // Create a rectangle shape of the capsule with the size of the capsule sprite
        capsuleRectangle.setPosition(position); // Set the position of the capsule rectangle to the position of the capsule
        capsuleRectangle.setFillColor(sf::Color::Transparent); // Set the fill color of the capsule rectangle to transparent
        capsuleRectangle.setOutlineColor(sf::Color::Red); // Set the outline color of the capsule rectangle to red
        capsuleRectangle.setOutlineThickness(1.f); // Set the outline thickness of the capsule rectangle to 1 pixel
    }
}

void Capsule::update(float dt) { // Updates the position of the capsule
    // Update the energy of the capsule
    energy += rechargeRate * dt;
    if (energy > maxEnergy) {
        energy = maxEnergy;
    }
    // Update the attack cooldown of the capsule
    attackCooldown -= dt;
    if (attackCooldown < 0) {
        attackCooldown = 0;
    }
    // TODO: TargetQueue integration handled later.
}

Capsule::Capsule(float health, float maxHealth, float energy, float maxEnergy, float rechargeRate, float attackCooldown, float attackRate, float attackRange, float damage, sf::Vector2f position, Enemy* target)
    : texture(),
      sprite(texture) {
    this->health = health;
    this->maxHealth = maxHealth;
    this->energy = energy;
    this->maxEnergy = maxEnergy;
    this->rechargeRate = rechargeRate;
    this->attackCooldown = attackCooldown;
    this->attackRate = attackRate;
    this->attackRange = attackRange;
    this->damage = damage;
    this->position = position;
    this->target = target;
    if (!texture.loadFromFile("assets/textures/capsule.png")) {
        std::cerr << "Failed to load capsule texture: assets/textures/capsule.png\n";
    } else {
        sprite.setTexture(texture, true);
        const auto bounds = sprite.getLocalBounds(); // Get the local bounds of the capsule sprite
        sprite.setOrigin({bounds.size.x * 0.5f, bounds.size.y * 0.5f}); // Set the origin of the capsule sprite to the center of the sprite
        sprite.setPosition(position); // Set the position of the capsule sprite to the position of the capsule
        sprite.setScale({0.3f, 0.3f}); // Set the scale of the capsule sprite to 0.3f        
        capsuleRectangle = sf::RectangleShape(sf::Vector2f(bounds.size.x, bounds.size.y)); // Create a rectangle shape of the capsule with the size of the capsule sprite
        capsuleRectangle.setPosition(position); // Set the position of the capsule rectangle to the position of the capsule
        capsuleRectangle.setFillColor(sf::Color::Transparent); // Set the fill color of the capsule rectangle to transparent
        capsuleRectangle.setOutlineColor(sf::Color::Red); // Set the outline color of the capsule rectangle to red
        capsuleRectangle.setOutlineThickness(1.f); // Set the outline thickness of the capsule rectangle to 1 pixel
    }
}

void Capsule::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Capsule::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    sprite.setPosition(position);
    capsuleRectangle.setPosition(position);
}

void Capsule::setScale(const sf::Vector2f& newScale) {
    sprite.setScale(newScale);
}

void Capsule::setAnchorBottomLeft(const sf::Vector2u& windowSize, float margin) {
    const auto bounds = sprite.getLocalBounds();
    const float halfW = bounds.size.x * 0.5f;
    const float halfH = bounds.size.y * 0.5f;
    const float x = margin + halfW;
    const float y = static_cast<float>(windowSize.y) - margin - halfH;
    setPosition({x, y});
}