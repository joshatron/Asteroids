#ifndef STAR_H
#define STAR_H
#include <glm/glm.hpp>
#include "object.h"
#include <vector>
#include <memory>

using glm::vec2;
using std::vector;
using std::shared_ptr;

class Star: public Object
{
    public:
        Star(vec2 center, vec2 vel, int index, double weight, double r);
        vector<shared_ptr<Object>> destroy(const GameState& state, int region);

        double radius;
};
#endif
