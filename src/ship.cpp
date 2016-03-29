#include "ship.h"
#include "bullet.h"

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
    friction = 40;
    bulletAge = 3;
    bulletSpeed = 250;
    fireRate = .15;
    teleportRate = .5; 
    maxBullets = 4;
}
