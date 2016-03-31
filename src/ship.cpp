#include "ship.h"
#include "bullet.h"

using std::make_shared;

Ship::Ship(vec2 location, int index):
Object(location, vec2(0,0), index)
{
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
    bulletAngle = 0;
    maxBullets = 4;
}

vector<shared_ptr<Object>> Ship::updateFromControls(GameState& state, double timePassed)
{
    vector<shared_ptr<Object>> objects;

    //turning left
    if(turningLeft && !turningRight)
    {
        angle -= turnRate * timePassed;
        if(angle < 0)
        {
            angle += 2 * PI;
        }
    }
    //turning right
    else if(turningRight && !turningLeft)
    {
        angle += turnRate * timePassed;
        if(angle > 2 * PI)
        {
            angle -= 2 * PI;
        }
    }
    //thrusting
    if(thrusting)
    {
        velocity.x += cos(angle - (PI / 2)) * (thrust * timePassed);
        velocity.y += sin(angle - (PI / 2)) * (thrust * timePassed);
    }
    else
    {
        double totalVel = distance(velocity, vec2(0,0));
        if(velocity.x != 0)
        {
            velocity.x -= (velocity.x / totalVel) * friction * timePassed;
        }
        if(velocity.y != 0)
        {
            velocity.y -= (velocity.y / totalVel) * friction * timePassed;
        }
    }
    //firing
    if(firing && bulletCooldowns.size() < (unsigned int)maxBullets && fireCooldown <= 0)
    {
        for(unsigned int a = 0; a < bulletFirePoints.size(); a++)
        {
            double dist = distance(bulletFirePoints.at(a), vec2(0,0));
            double ang = atan(bulletFirePoints.at(a).y / bulletFirePoints.at(a).x);
            vec2 point;
            point.x = position.x + (dist * cos(angle + ang));
            point.y = position.y + (dist * sin(angle + ang));
            vec2 vel;
            vel.x = cos((angle + bulletAngle) - (PI / 2)) * bulletSpeed + velocity.x;
            vel.y = sin((angle + bulletAngle) - (PI / 2)) * bulletSpeed + velocity.y;
            shared_ptr<Bullet> bullet(make_shared<Bullet>(point, vel, collisionIndex, bulletAge));
            objects->push_back(bullet);
        }
        
        fireCooldown = fireRate;
        bulletCooldowns.push_back(bulletAge);
    }
    //teleporting
    if(teleporting && teleportCooldown <= 0)
    {
        position.x = abs(rand() % state.width);
        position.y = abs(rand() % state.height);
        velocity.x = 0;
        velocity.y = 0;
        teleportCooldown = teleportRate;
    }

    return objects;
}

void Ship::updateTimers(double timePassed)
{
    fireCooldown -= timePassed;
    teleportCooldown -= timePassed;
    for(unsigned int a = 0; a < bulletCooldowns.size(); a++)
    {
        bulletCooldowns.at(a) -= timePassed;
        if(bulletCooldowns.at(a) <= 0)
        {
            bulletCooldowns.erase(bulletCooldowns.begin() + a);
        }
    }

}
