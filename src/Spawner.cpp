#include "Spawner.h"

Spawner::Spawner(float intervalSeconds)
    : timer(0.0f), spawnInterval(intervalSeconds) {}

void Spawner::update(float dt) {
    timer += dt;
}

bool Spawner::shouldSpawn() const {
    return timer >= spawnInterval;
}

void Spawner::consumeSpawn() {
    timer -= spawnInterval;
    if (timer < 0.0f) timer = 0.0f;
}