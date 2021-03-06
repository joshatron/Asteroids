#ifndef SHIP_H
#define SHIP_H
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "object.h"
#include "gamestate.h"

using glm::vec2;
using std::vector;
using std::shared_ptr;

class Ship: public Object
{
    public:
        Ship(vec2 location, int index, int type);
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

        vector<vec2> bulletFirePoints;
};

#endif
