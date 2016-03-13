#include "main_ship.h"

MainShip::MainShip(vec2 location)
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
}
