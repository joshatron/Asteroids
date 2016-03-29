#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <vector>
#include "bullet.h"
#include "asteroid.h"
#include "star.h"
#include "ship.h"
#include "animation.h"
#include "stats.h"

using std::vector;

class GameState
{
    public:
        vector<Ship> ships;
        vector<Bullet> bullets;
        vector<Asteroid> asteroids;
        vector<Star> stars;
        vector<Animation> animations;
        int nextNumAsteroids;
        double pauseTime;
        double floatTime;
        Stats stats;
};
#endif
