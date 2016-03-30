#ifndef MAIN_SHIP_H
#define MAIN_SHIP_H
#include <glm/glm.hpp>
#include "ship.h"

using glm::vec2;

class MainShip: public Ship
{
    public:
        void keyUpdate(int key, bool pressed);
        MainShip(vec2 location);
};
#endif
