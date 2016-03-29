#ifndef STAR_H
#define STAR_H
#include <glm/glm.hpp>
#include <vector>
#include "object.h"

using glm::vec2;
using std::vector;

class Star: public Object
{
    public:
        double radius;
        double gravity;
};
#endif
