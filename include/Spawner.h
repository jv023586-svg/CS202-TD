#pragma once

#include <SFML/Graphics.hpp>

/// Fixed world object: timer logic for spawning enemies, drawn like Capsule (sprite + debug hitbox, one visual scale).
class Spawner {
public:
    Spawner();

    void update(float dt);
    bool shouldSpawn() const;
    void consumeSpawn();

    void draw(sf::RenderWindow& window);
    void setPosition(const sf::Vector2f& newPosition);
    void setVisualScale(const sf::Vector2f& newScale);
    sf::Vector2f getPosition() const;

private:
    void syncDrawablesAfterTextureLoad();

    float timer = 0.0f;
    float spawnInterval = 2.0f;

    sf::Vector2f position;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RectangleShape spawnerHitbox;
    sf::Vector2f visualScale{3.0f, 3.0f};
};
