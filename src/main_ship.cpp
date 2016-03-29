#include "main_ship.h"
#include <glm/glm.hpp>
#include <iostream>

using std::cout;
using std::endl;
using glm::distance;

MainShip::MainShip(vec2 location) : Ship(location)
{
    points.push_back(vec2(0, -15));
    points.push_back(vec2(8, 15));
    points.push_back(vec2(8, 15));
    points.push_back(vec2(4, 10));
    points.push_back(vec2(4, 10));
    points.push_back(vec2(-4, 10));
    points.push_back(vec2(-4, 10));
    points.push_back(vec2(-8, 15));
    points.push_back(vec2(-8, 15));
    points.push_back(vec2(0, -15));
    
    collisionShapes.push_back(ConvexShape());
    collisionShapes.at(0).points.push_back(vec2(0, -15));
    collisionShapes.at(0).points.push_back(vec2(8, 15));
    collisionShapes.at(0).points.push_back(vec2(-8, 15));
    
    shipFirePoints.push_back(vec2(4, 10));
    shipFirePoints.push_back(vec2(0, 20));
    shipFirePoints.push_back(vec2(0, 20));
    shipFirePoints.push_back(vec2(-4, 10));
    
    bulletFirePoint.x = 0;
    bulletFirePoint.y = -16;

    friction = 40;
}

void MainShip::updateVelocity(double timePassed)
{
    Ship::updateVelocity(timePassed);
    cout << "hi" << endl;
    if(!thrusting)
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
}
