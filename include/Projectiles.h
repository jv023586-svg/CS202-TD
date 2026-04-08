#pragma once

/*
Projectile
- position: sf::Vector2f
- velocity: sf::Vector2f
- damage : float
- isActive : bool
- target : Enemy*

+ Projectile()
+ update(dt:float):void
+ draw(window):void
+ isAlive():bool

*/

#include <SFML/Graphics.hpp>
#include "Enemy.h"

class Projectile {
public:
    Projectile();
    void update(float dt);
    void draw(window);
    bool isAlive();

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float damage;
    bool isActive;
    Enemy* target;
};
