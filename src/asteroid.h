#ifndef ASTEROID_H
#define ASTEROID_H
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "object.h"

using glm::vec2;
using std::vector;
using std::shared_ptr;

class Asteroid: public Object
{
    public:
        Asteroid(vec2 center, double radius, double velocity);
        vector<shared_ptr<Object>> destroy(const GameState& state, int region);

        double radius;
};
#endif
