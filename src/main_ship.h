#ifndef MAIN_SHIP_H
#define MAIN_SHIP_H
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "ship.h"

using glm::vec2;
using std::vector;
using std::shared_ptr;

class MainShip: public Ship
{
    public:
        MainShip(vec2 location, int index);
        void keyUpdate(int key, bool pressed);
        vector<shared_ptr<Object>> destroy(const GameState& state, int region);
};
#endif
