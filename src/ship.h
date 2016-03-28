#ifndef SHIP_H
#define SHIP_H
#include <glm/glm.hpp>
#include "object.h"
#include "gamestate.h"

using glm::vec2;

class Ship: public Object
{
    public:
        Ship(vec2 location);
        virtual void updateVelocity(GameState& state, double timePassed);
        virtual void fire(GameState& state);

        double fireCooldown;
        double teleportCooldown;

        bool turningLeft;
        bool turningRight;
        bool thrusting;
        bool firing;
        bool teleporting;

        double turnRate;
        double thrust;
        double bulletAge;
        double bulletSpeed;
        double fireRate;
        double teleportRate;
        int maxBullets;

        vector<double> bulletCooldowns;

        //put in individual lines, not a line loop or strip
        vector<vec2> shipFirePoints;

        vec2 bulletFirePoint;
};
#endif
