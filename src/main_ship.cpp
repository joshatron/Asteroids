#include "main_ship.h"
#include <glm/glm.hpp>

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
