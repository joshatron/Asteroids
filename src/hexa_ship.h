#ifndef HEXA_SHIP_H
#define HEXA_SHIP_H
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "ship.h"

using glm::vec2;
using std::vector;
using std::shared_ptr;

class HexaShip: public Ship
{
    public:
        HexaShip(vec2 location, int index);
        vector<shared_ptr<Object>> destroy(const GameState& state, int region);
    private:
        shared_ptr<Object> target;
};
#endif
