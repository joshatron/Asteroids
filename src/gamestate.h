#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <glm/glm.hpp>
#include <vector>
#include "bullet.h"
#include "asteroid.h"

using glm::vec2;
using std::vector;

class GameState
{
    public:
        vec2 *shipLoc;
        vec2 *shipVelocity;
        double shipAngle;
        int livesLeft;
        int score;
        int numAsteroids;
        vector<Bullet> *bullets;
        vector<Asteroid> *asteroids;
        double pauseTime;
        double lastFiredCooldown;
        double teleportCooldown;

        bool turningLeft;
        bool turningRight;
        bool thrusting;
        bool firing;
        bool teleporting;
};
#endif
