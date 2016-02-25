#ifndef BULLET_H
#define BULLET_H
#include <glm/glm.hpp>

using glm::vec2;

class Ship
{
    public:
        vec2 position;
        vec2 velocity;
        double angle;

        double fireCooldown;
        double teleportCooldown;

        int lives;
        int score;

        bool turningLeft;
        bool turningRight;
        bool thrusting;
        bool teleporting;
        bool firing;
        bool fired;

        //put in individual lines, not a line loop or strip
        vector<vec2> shipPoints;
        vector<vec2> shipFirePoints;
        vec2 bulletFirePoint;
};
#endif
