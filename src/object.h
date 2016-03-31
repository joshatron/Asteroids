#ifndef OBJECT_H
#define OBJECT_H
#include <vector>
#include <memory>
#include "convex_shape.h"

#ifndef PI
    #define PI 3.14159265
#endif

using glm::distance;
using std::vector;
using std::shared_ptr;

class GameState;
class Object
{
    public:
        Object(vec2 center, vec2 vel, int index): mass(0), position(center), velocity(vel), angle(0), timeToLive(9999),
                                                  period(0), percent(0), collisionIndex(index), drawPoints(false) {};
        //methods that the objects will use to do more than float around
        //does not change state, just control flags
        virtual void update(const GameState& state) {};
        virtual void keyUpdate(int key, bool pressed) {};
        virtual vector<shared_ptr<Object>> updateFromControls(const GameState& state, double timePassed) {return vector<shared_ptr<Object>>();}
        virtual void updateTimers(double timePassed) {timeToLive -= timePassed;}
        virtual vector<shared_ptr<Object>> destroy(const GameState& state, int region) {return vector<shared_ptr<Object>>();}

        //general info about them
        double mass;
        vec2 position;
        vec2 velocity;
        double angle;
        vector<ConvexShape> collisionShapes;
        vector<vec2> points;

        vector<vec2> tempPoints;
        double period;
        double percent;

        double timeToLive;
        
        //objects of the same index pass through each other
        //objects with a -1 index don't interact with anything
        int collisionIndex;

        int typeIndex;

        bool drawPoints;
};

#endif
