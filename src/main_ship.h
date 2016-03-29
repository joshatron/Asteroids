#ifndef MAIN_SHIP_H
#define MAIN_SHIP_H
#include <glm/glm.hpp>

using glm::vec2;

class MainShip: public Ship
{
    public:
        MainShip(vec2 location);
};
#endif
