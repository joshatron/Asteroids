#ifndef MAIN_SHIP_H
#define MAIN_SHIP_H
#include <glm/glm.hpp>
#include "ship.h"


using glm::vec2;

class MainShip: public Ship
{
    public:
        MainShip(vec2 location);
        void updateVelocity(GameState& state, double timePassed);
        void fire(GameState& state);
    private:
        double friction;
};
#endif
