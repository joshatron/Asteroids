#include "main_ship.h"
#include "convex_shape.h"
#include "animation.h"
#include <glm/glm.hpp>
#include <QTextStream>

using std::endl;
using glm::distance;
using std::make_shared;

MainShip::MainShip(vec2 location, int index):
Ship(location, index, 0)
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
    
    tempPoints.push_back(vec2(4, 10));
    tempPoints.push_back(vec2(0, 20));
    tempPoints.push_back(vec2(0, 20));
    tempPoints.push_back(vec2(-4, 10));

    period = 0;
    percent = .5;
    
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
                period = .16;
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
                period = 0;
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

vector<shared_ptr<Object>> MainShip::destroy(const GameState& state, int region)
{
    vector<shared_ptr<Object>> toReturn;

    toReturn.push_back(make_shared<Animation>(position, vec2(15,-4), vec2(0,-15),vec2(8,15), .75));
    toReturn.push_back(make_shared<Animation>(position, vec2(-15,-4), vec2(0,-15),vec2(-8,15), .75));
    toReturn.push_back(make_shared<Animation>(position, vec2(8,18), vec2(4,10),vec2(8,15), .75));
    toReturn.push_back(make_shared<Animation>(position, vec2(-8,18), vec2(-4,10),vec2(-8,15), .75));
    toReturn.push_back(make_shared<Animation>(position, vec2(0,20), vec2(4,10),vec2(-4,10), .75));
    
    return toReturn;
}
