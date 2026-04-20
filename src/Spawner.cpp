#include "Spawner.h"
#include <iostream>

Spawner::Spawner()
    : timer(0.0f),
      spawnInterval(2.0f),
      position({1700.f, 300.f}),
      texture(),
      sprite(texture) {
    if (!texture.loadFromFile("assets/textures/spawner.png")) {
        std::cerr << "Failed to load spawner texture: assets/textures/spawner.png\n";
    } else {
        syncDrawablesAfterTextureLoad();
    }
}

void Spawner::syncDrawablesAfterTextureLoad() {
    sprite.setTexture(texture, true);
    const auto bounds = sprite.getLocalBounds();
    const float hx = bounds.size.x * 0.5f; // half of the width of the sprite
    const float hy = bounds.size.y * 0.5f; // half of the height of the sprite
    sprite.setOrigin({hx, hy});
    sprite.setPosition(position);
    spawnerHitbox.setSize({bounds.size.x, bounds.size.y});
    spawnerHitbox.setOrigin({hx, hy});
    spawnerHitbox.setPosition(position);
    spawnerHitbox.setFillColor(sf::Color::Transparent);
    spawnerHitbox.setOutlineColor(sf::Color::Red);
    spawnerHitbox.setOutlineThickness(1.5f);
    setVisualScale(visualScale);
}

void Spawner::update(float dt) {
    timer += dt;
}

bool Spawner::shouldSpawn() const {
    return timer >= spawnInterval;
}

void Spawner::consumeSpawn() {
    timer -= spawnInterval;
    if (timer < 0.0f) {
        timer = 0.0f;
    }
}

void Spawner::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(spawnerHitbox);
}

void Spawner::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    sprite.setPosition(position);
    spawnerHitbox.setPosition(position);
}

void Spawner::setVisualScale(const sf::Vector2f& newScale) {
    visualScale = newScale;
    sprite.setScale(visualScale);
    spawnerHitbox.setScale(visualScale);
}

sf::Vector2f Spawner::getPosition() const {
    return position;
}
