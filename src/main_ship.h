#ifndef MAIN_SHIP_H
#define MAIN_SHIP_H
#include <glm/glm.hpp>
#include "ship.h"
#include "bullet.h"


using glm::vec2;

class MainShip: public Ship
{
    public:
        MainShip(vec2 location);
        virtual void updateVelocity(double timePassed);
    private:
        double friction;
};
#endif
