#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <glm/glm.hpp>
#include <vector>
#include "bullet.h"
#include "asteroid.h"
#include "ship.h"

using glm::vec2;
using std::vector;

class GameState
{
    public:
        vector<Ship> ships;
        vector<Bullet> bullets;
        vector<Asteroid> asteroids;
        int nextNumAsteroids;
        double pauseTime;
};
#endif
