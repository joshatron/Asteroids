#ifndef ASTEROID_H
#define ASTEROID_H
#include <glm/glm.hpp>
#include <vector>

using glm::vec2;
using std::vector;

class Asteroid
{
    public:
        vec2 position;
        vec2 velocity;
        double radius;
        vector<vec2> points;
};
#endif
