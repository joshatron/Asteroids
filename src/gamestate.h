#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <glm/glm.hpp>
#include <vector>
#include "bullet.h"
#include "asteroid.h"
#include "ship.h"
#include "animation.h"
#include "stats.h"

using glm::vec2;
using std::vector;

class GameState
{
    public:
        vector<Ship> ships;
        vector<Bullet> bullets;
        vector<Asteroid> asteroids;
        vector<Animation> animations;
        int nextNumAsteroids;
        double pauseTime;
        double playTime;
        vector<int> shipIndexes;
        Stats stats;
};
#endif
