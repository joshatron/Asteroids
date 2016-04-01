#include "hexa_ship.h"
#include "convex_shape.h"
#include "animation.h"
#include <glm/glm.hpp>

using glm::distance;
using std::make_shared;

HexaShip::HexaShip(vec2 location, int index):
Ship(location, index, 3)
{
    //border
    points.push_back(vec2(0, -35));
    points.push_back(vec2(30.31, -17.45));
    points.push_back(vec2(30.31, -17.45));
    points.push_back(vec2(30.31, 17.45));
    points.push_back(vec2(30.31, 17.45));
    points.push_back(vec2(0, 35));
    points.push_back(vec2(0, 35));
    points.push_back(vec2(-30.31, 17.45));
    points.push_back(vec2(-30.31, 17.45));
    points.push_back(vec2(-30.31, -17.45));
    points.push_back(vec2(-30.31, -17.45));
    points.push_back(vec2(0, -35));

    //spokes
    points.push_back(vec2(30.31, -17.45));
    points.push_back(vec2(0, 0));
    points.push_back(vec2(0, 35));
    points.push_back(vec2(0, 0));
    points.push_back(vec2(-30.31, -17.45));
    points.push_back(vec2(0, 0));

    //ship backs
    points.push_back(vec2(0, 0));
    points.push_back(vec2(-2.284, -17.35));
    points.push_back(vec2(0, 0));
    points.push_back(vec2(2.284, -17.35));
    points.push_back(vec2(-2.284, -17.35));
    points.push_back(vec2(0, -35));
    points.push_back(vec2(2.284, -17.35));
    points.push_back(vec2(0, -35));

    points.push_back(vec2(0, 0));
    points.push_back(vec2(16.168, 6.697));
    points.push_back(vec2(0, 0));
    points.push_back(vec2(13.884, 10.653));
    points.push_back(vec2(16.168, 6.697));
    points.push_back(vec2(30.31, 17.45));
    points.push_back(vec2(13.884, 10.653));
    points.push_back(vec2(30.31, 17.45));

    points.push_back(vec2(0, 0));
    points.push_back(vec2(-16.168, 6.697));
    points.push_back(vec2(0, 0));
    points.push_back(vec2(-13.884, 10.653));
    points.push_back(vec2(-16.168, 6.697));
    points.push_back(vec2(-30.31, 17.45));
    points.push_back(vec2(-13.884, 10.653));
    points.push_back(vec2(-30.31, 17.45));

    //collision points
    collisionShapes.push_back(ConvexShape());
    collisionShapes.at(0).points.push_back(vec2(0, -35));
    collisionShapes.at(0).points.push_back(vec2(30.31, -17.45));
    collisionShapes.at(0).points.push_back(vec2(30.31, 17.45));
    collisionShapes.at(0).points.push_back(vec2(0, 35));
    collisionShapes.at(0).points.push_back(vec2(-30.31, 17.45));
    collisionShapes.at(0).points.push_back(vec2(-30.31, -17.45));
    
    period = 0;
    percent = 0;
}

vector<shared_ptr<Object>> HexaShip::destroy(const GameState& state, int region)
{
    vector<shared_ptr<Object>> toReturn;

    return toReturn;
}
