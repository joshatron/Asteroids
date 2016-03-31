#ifndef ASTEROID_H
#define ASTEROID_H
#include "object.h"

class Asteroid: public Object
{
    public:
        Asteroid(vec2 center, double radius, double velocity);
        vector<shared_ptr<Object>> destroy(const GameState& state, int region);

        double radius;
};
#endif
