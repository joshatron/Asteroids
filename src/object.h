#ifndef OBJECT_H
#define OBJECT_H
#include <vector>
#include "convex_shape.h"

#ifndef PI
    #define PI 3.14159265
#endif


using glm::distance;
using std::vector;

class GameState;
class Object
{
    public:
        //methods that the objects will use to do more than float around
        //does not change state, just control flags
        virtual void update(GameState& state) {};
        virtual void keyUpdate(int key, bool pressed) {};

        //general info about them
        vec2 position;
        vec2 velocity;
        double angle;
        vector<ConvexShape> collisionShapes;
        vector<vec2> points;
};

#endif
