#pragma once

class Spawner {
    public:
        Spawner(float intervalSeconds = 2.0f);

        void update(float dt);
        bool shouldSpawn() const;
        void consumeSpawn();

    private:
        float timer = 0.0f;
        float spawnInterval = 2.0f;
};