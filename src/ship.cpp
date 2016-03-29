#include "ship.h"
#include "bullet.h"

#ifndef PI
    #define PI 3.14159265
#endif

Ship::Ship(vec2 location)
{
    position.x = location.x;
    position.y = location.y;
    velocity.x = 0;
    velocity.y = 0;
    angle = 0;

    fireCooldown = 0;
    teleportCooldown = 0;

    turningLeft = false;
    turningRight = false;
    thrusting = false;
    firing = false;
    teleporting = false;

    turnRate = 5;
    thrust = 300;
    bulletAge = 3;
    bulletSpeed = 250;
    fireRate = .15;
    teleportRate = .5; 
    maxBullets = 4;
}

void Ship::updateVelocity(double timePassed)
{
    if(thrusting)
    {
        velocity.x += cos(angle - (PI / 2)) * (thrust * timePassed);
        velocity.y += sin(angle - (PI / 2)) * (thrust * timePassed);
    }
}

Bullet Ship::fire()
{
    double dist = distance(bulletFirePoint, vec2(0,0));
    double ang = atan(bulletFirePoint.y / bulletFirePoint.x);
    Bullet bullet;
    bullet.position.x = position.x + (dist * cos(angle + ang));
    bullet.position.y = position.y + (dist * sin(angle + ang));
    bullet.velocity.x = cos(angle - (PI / 2)) * bulletSpeed + velocity.x;
    bullet.velocity.y = sin(angle - (PI / 2)) * bulletSpeed + velocity.y;
    bullet.age = bulletAge;
    return bullet;
}
