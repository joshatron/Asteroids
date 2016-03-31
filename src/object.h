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
        Object(vec2 center, vec2 vel, int index): position(center), velocity(vel), collisionIndex(index), weigth(0),
                                                  angle(0), timeToLive(9999), collisionIndex(-1), point(false) {};
        //methods that the objects will use to do more than float around
        //does not change state, just control flags
        virtual void update(const GameState& state) {};
        virtual void keyUpdate(int key, bool pressed) {};
        virtual vector<shared_ptr<Object>> updateFromControls(const GameState& state, double timePassed) {return vector<shared_ptr<Object>>;};
        virtual void updateTimers(double timePassed) {timeToLive -= timePassed;};
        virtual vector<shared_ptr<Object>> destroy(const GameState& state, int region) {return vector<shared_ptr<Object>>;};

        //general info about them
        double weight;
        vec2 position;
        vec2 velocity;
        double angle;
        vector<ConvexShape> collisionShapes;
        vector<vec2> points;

        double timeToLive;
        
        //objects of the same index pass through each other
        //objects with a -1 index don't interact with anything
        int collisionIndex;
};

#endif
