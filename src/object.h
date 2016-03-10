#ifndef OBJECT_H
#define OBJECT_H
#include <glm/glm.hpp>
#include <vector>
#include "concave_shape.h"

using glm::vec2;
using std::vector;

class Object
{
    public:
        vec2 position;
        vec2 velocity;
        double angle;
        vector<vec2> points;
        vector<ConcaveShape> collisionShapes;
};

#endif
