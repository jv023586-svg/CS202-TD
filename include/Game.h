#pragma once

#include <SFML/Graphics.hpp>
#include "Capsule.h"
// #include "Enemy.h"
// #include "Turret.h"
// #include "Projectile.h"
// #include "TargetQueue.h"
// #include "Spawner.h"

class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    Capsule capsule;

    // std::vector<Enemy> enemies;
    // std::vector<Turret> turrets;
    // std::vector<Projectile> projectiles;
    // TargetQueue targetQueue;
    // Spawner spawner;

    bool gameOver;
    bool gameWon;

    void processEvents();
    void update();
    void render();
};