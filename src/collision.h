#ifndef COLLISION_H
#define COLLISION_H
#include <glm/glm.hpp>
#include <memory>
#include "object.h"

using glm::vec2;
using std::shared_ptr;

class CollisionDetection
{
    public:
        //detects collisions between 2 objects
        //the return is -1 if no collision,
        //otherwise it is index of the shape that collided on the first object
        static int twoShapes(shared_ptr<Object> first, shared_ptr<Object> second);
        //detects collisions between an object and a point
        //the return is -1 if no collision,
        //otherwise it is index of the shape that collided on the object
        //Uses the ray casting algorithm
        static int shapeAndPoint(shared_ptr<Object> object, vec2 point);

    private:
        static bool rayIntersects(vec2 point, vec2 start, vec2 end);
        static void transform(vec2& point, vec2 transform, double angle);
};
#endif
