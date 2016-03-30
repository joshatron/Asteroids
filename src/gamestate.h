#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <vector>
#include <memory>
#include "bullet.h"
#include "asteroid.h"
#include "star.h"
#include "ship.h"
#include "animation.h"
#include "stats.h"

using std::vector;
using std::shared_ptr;

class GameState
{
    public:
        vector<shared_ptr<Ship>> ships;
        vector<shared_ptr<Bullet>> bullets;
        vector<shared_ptr<Asteroid>> asteroids;
        vector<shared_ptr<Star>> stars;
        vector<shared_ptr<Animation>> animations;
        int nextNumAsteroids;
        double pauseTime;
        double floatTime;
        shared_ptr<Stats> stats;
};
#endif
