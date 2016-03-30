#include "main_ship.h"
#include <glm/glm.hpp>
#include <QTextStream>

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
    
    bulletFirePoints.push_back(vec2(0, -16));

    friction = 40;
}

void MainShip::keyUpdate(int key, bool pressed)
{
    if(pressed)
    {
        switch(key)
        {
            case Qt::Key_D:
                turningLeft = true;
                turningRight = false;
                break;
            case Qt::Key_F:
                turningLeft = false;
                turningRight = true;
                break;
            case Qt::Key_J:
                firing = true;
                break;
            case Qt::Key_K:
                thrusting = true;
                break;
            case Qt::Key_L:
                teleporting = true;
                break;
        }
    }
    else
    {
        switch(key)
        {
            case Qt::Key_D:
                turningLeft = false;
                break;
            case Qt::Key_F:
                turningRight = false;
                break;
            case Qt::Key_K:
                thrusting = false;
                break;
            case Qt::Key_J:
                firing = false;
                break;
            case Qt::Key_L:
                teleporting = false;
                break;
        }
    }
}
