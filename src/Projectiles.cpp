#include "Projectiles.h"

/*
    // variables
    sf::Vector2f position;
    sf::Vector2f velocity;
    float damage;
    bool isActive;
    Enemy* target;
*/
Projectile::Projectile()
{
    position = sf::Vector2f(0.f, 0.f);  // set position 0.f, 0.f is top left corner
    velocity = sf::Vector2f(0.f, 0.f);  // set velocity, no current speed
    damage = 0.f;                       // set damage
    isActive = false;                   // set active state, false prevents accidental updates
    target = NULL;                      // set target
}

void update(float dt)
{
// frame time
// actual time dt is change in time

    if (!isActive)  // will return if active state isn't updated between creation and here
    {
        return; 
    }

    // update position--move projectile
    position += velocity * dt;

    if (position.x < 0 || position.x > 800 ||  //change to window's max x
        position.y < 0 || position.y > 600)    //change to window's max y
    {
        isActive = false;
    }

    // update velocity
    

    // take damage


    // update/check alive state


    // target

}

void draw(sf::RenderWindow& window)
{

}

bool isAlive()
{

}

