#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <vector>
#include <memory>
#include "object.h"
#include "stats.h"

using std::vector;
using std::shared_ptr;

class GameState
{
    public:
        vector<shared_ptr<Object>> objects;
        int nextNumAsteroids;
        double pauseTime;
        double floatTime;
        int width;
        int height;
        double asteroidSpeed;
        double baseAsteroidRadius;
        double gravityConstant;
        shared_ptr<Stats> stats;
};
#endif
