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

    if (!isActive())  // check alive or not. Will return if active state isn't updated between creation and here
    {
        return; 
    }

    // deactivate projectile if out of window
    if (position.x < 0 || position.x > 800 ||  //change to window's max x
        position.y < 0 || position.y > 600)    //change to window's max y
    {
        isActive = false;
        return;
    }

    // direction to target
    sf::Vector2f direction = target->getPosition() - position;

    // Compute length (magnitude)
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    // Normalize projectile speed
    if (length != 0.f)
    {
        direction /= length;
    }

    float speed = 200.f; // units per second

    // Update velocity toward target
    velocity = direction * speed;

    // update position--move projectile
    position += velocity * dt;

    // take damage
    if (target != NULL)
    {
        if (/* collision check with target */)
        {
            target->takeDamage(damage);
            isActive = false;
        }
    }
}

void draw(sf::RenderWindow& window)
{
    if (!isActive())  // check alive or not. Will return if active state isn't updated between creation and here
    {
        return;
    }

    // draw circle (placeholder for sprite) and set position and color
    sf::CircleShape shape(5.f); // radius = 5
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Red);

    window.draw(shape);
}

bool isAlive()
{
    return isActive;
}

