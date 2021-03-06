#include "star.h"
#include "convex_shape.h"

using std::make_shared;

Star::Star(vec2 center, vec2 vel, int index, double weight, double r):
Object(center, vel, index, 0)
{
    mass = weight;
    radius = r;
    vector<vec2> allPoints;
    for(double k = 0; k < (2 * PI) - .0001; k += (PI / 6))
    {
        allPoints.push_back(vec2(r * cos(k), r * sin(k)));
    }

    for(int k = 0; k < 6; k++)
    {
        points.push_back(allPoints.at(k));
        points.push_back(allPoints.at(k + 6));
    }

    ConvexShape shape;
    shape.points = allPoints;
    collisionShapes.push_back(shape);
}

vector<shared_ptr<Object>> Star::destroy(const GameState& state, int region)
{
    vector<shared_ptr<Object>> toReturn;
    toReturn.push_back(make_shared<Star>(position, velocity, collisionIndex, mass, radius));
    return toReturn;
}
