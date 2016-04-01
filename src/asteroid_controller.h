#ifndef ASTR_CTRLR_H
#define ASTR_CTRLR_H
#include "controller.h"
#include "gamestate.h"

//count indexes
//0- dont care
//1- main ship
//2- asteroids
//3- hexaship
class AsteroidsController: public Controller
{
    public:
        AsteroidsController();
        void initialize(GameState& state);
        void updateCount(GameState& state, int index, bool add);
        void updateTimers(GameState& state, double timePassed);
        int lives;
        int score;

        double basePauseTime;
        double baseDeathTime;
        int baseAsteroids;
        int currentIndex;

    private:
        void createAsteroids(GameState& state, int number);
        void updateScore(int points);

        double timeToReset;
};

#endif
