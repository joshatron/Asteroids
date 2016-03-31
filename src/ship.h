#ifndef SHIP_H
#define SHIP_H
#include <glm/glm.hpp>
#include <vector>
#include "object.h"

using glm::vec2;
using std::vector;

class Ship: public Object
{
    public:
        Ship(vec2 location, int index);
        vector<shared_ptr<Object>> updateFromControls(const GameState& state, double timePassed);
        void updateTimers(double timePassed);

        double fireCooldown;
        double teleportCooldown;

        bool turningLeft;
        bool turningRight;
        bool thrusting;
        bool firing;
        bool teleporting;

        double turnRate;
        double thrust;
        double friction;
        double bulletAge;
        double bulletSpeed;
        double fireRate;
        double teleportRate;
        double bulletAngle;
        int maxBullets;

        vector<double> bulletCooldowns;

        //put in individual lines, not a line loop or strip
        vector<vec2> shipFirePoints;

        vector<vec2> bulletFirePoints;
};

#endif
