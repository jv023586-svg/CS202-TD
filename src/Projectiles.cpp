#include "Projectiles.h"
#include <cmath>
#include <iostream>

namespace {
constexpr float kArriveEpsilon = 18.f;
}

Projectile::Projectile(sf::Vector2f from, sf::Vector2f to, float speedPixelsPerSecond, sf::Vector2f scale)
    : start(from),
      end(to),
      position(from),
      texture(),
      sprite(texture),
      visualScale(scale) {
    const sf::Vector2f delta = to - from;
    const float len = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    totalDistance = len;
    if (len < 0.001f || speedPixelsPerSecond <= 0.f) {
        active = false;
        return;
    }
    sf::Vector2f dir = delta / len;
    velocity = dir * speedPixelsPerSecond;

    if (!texture.loadFromFile("assets/textures/projectile.png")) {
        std::cerr << "Failed to load projectile texture: assets/textures/projectile.png\n";
        active = false;
        return;
    }

    sprite.setTexture(texture, true);
    // SFML may reset scale when the texture is bound; apply scale again after origin/rotation are set.
    const auto bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x * 0.5f, bounds.size.y * 0.5f});
    sprite.setPosition(position);
    aimSpriteAlongVelocity();
    setVisualScale(visualScale);
}

Projectile::Projectile(Projectile&& other) noexcept
    : start(other.start),
      end(other.end),
      position(other.position),
      velocity(other.velocity),
      totalDistance(other.totalDistance),
      active(other.active),
      texture(std::move(other.texture)),
      sprite(std::move(other.sprite)),
      visualScale(other.visualScale) {
    sprite.setTexture(texture, false);
    sprite.setPosition(position);
    setVisualScale(visualScale);
    aimSpriteAlongVelocity();
}

Projectile& Projectile::operator=(Projectile&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    start = other.start;
    end = other.end;
    position = other.position;
    velocity = other.velocity;
    totalDistance = other.totalDistance;
    active = other.active;
    texture = std::move(other.texture);
    sprite = std::move(other.sprite);
    visualScale = other.visualScale;
    sprite.setTexture(texture, false);
    sprite.setPosition(position);
    setVisualScale(visualScale);
    aimSpriteAlongVelocity();
    return *this;
}

void Projectile::setVisualScale(const sf::Vector2f& newScale) {
    visualScale = newScale;
    sprite.setScale(visualScale);
}

void Projectile::aimSpriteAlongVelocity() {
    const float rad = std::atan2(velocity.y, velocity.x);
    sprite.setRotation(sf::radians(rad));
}

void Projectile::update(float dt) {
    if (!active) {
        return;
    }
    position += velocity * dt;
    sprite.setPosition(position);

    const sf::Vector2f toEnd = end - position;
    const float distEnd = std::sqrt(toEnd.x * toEnd.x + toEnd.y * toEnd.y);
    if (distEnd < kArriveEpsilon) {
        active = false;
        return;
    }
    const sf::Vector2f traveled = position - start;
    const float along = std::sqrt(traveled.x * traveled.x + traveled.y * traveled.y);
    if (along >= totalDistance - kArriveEpsilon) {
        active = false;
    }
}

void Projectile::draw(sf::RenderWindow& window) {
    if (!active) {
        return;
    }
    sprite.setScale(visualScale);
    window.draw(sprite);
}

bool Projectile::isActive() const {
    return active;
}
