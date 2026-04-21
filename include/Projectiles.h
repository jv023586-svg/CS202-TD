#pragma once

#include <SFML/Graphics.hpp>

/// Cosmetic only: travels in a straight line from `from` toward fixed `to` (snapshot). Does not apply damage.
/// Visual size is set only by the caller (see `kProjectileVisualScale` in Game.cpp).
class Projectile {
public:
    Projectile(sf::Vector2f from, sf::Vector2f to, float speedPixelsPerSecond, sf::Vector2f visualScale);

    Projectile(const Projectile&) = delete;
    Projectile& operator=(const Projectile&) = delete;
    Projectile(Projectile&& other) noexcept;
    Projectile& operator=(Projectile&& other) noexcept;

    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool isActive() const;
    void setVisualScale(const sf::Vector2f& newScale);

private:
    void aimSpriteAlongVelocity();

    sf::Vector2f start;
    sf::Vector2f end;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float totalDistance{0.f};
    bool active{true};
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f visualScale;
};
