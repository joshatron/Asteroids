#ifndef SHIP_H
#define SHIP_H
#include <glm/glm.hpp>
#include "object.h"

using glm::vec2;

class Ship: public Object
{
    public:
        double fireCooldown;
        double teleportCooldown;

        bool turningLeft;
        bool turningRight;
        bool thrusting;
        bool firing;
        bool teleporting;

        vector<double> bulletCooldowns;

        //put in individual lines, not a line loop or strip
        vector<vec2> shipFirePoints;

        vec2 bulletFirePoint;
};
#endif
