#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <vector>
#include "gamestate.h"

using std::vector;

class Controller
{
    public:
        virtual void initialize(GameState& state) = 0;
        virtual void updateCount(GameState& state, int index, bool add) = 0;
        virtual void updateTimers(GameState& state, double timePassed) = 0;

        double pauseTime;
        double floatTime;

        int width;
        int height;
        vector<int> counts;
};

#endif
