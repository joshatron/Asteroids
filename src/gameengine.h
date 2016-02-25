#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <glm/glm.hpp>
#include "gamestate.h"
#include "bullet.h"
#include "asteroid.h"

using glm::vec2;
using std::vector;

class GameEngine
{
    public:
        GameEngine();
        void createInitialState(GameState& state);
        void getNextState(GameState& state, double timePassed);
    private:
        void createAsteroids(GameState& state, int number);
        void createAsteroid(GameState& state, vec2 center, double radius, double velocity);
        void updateObjects(GameState& state, double timePassed);
        void detectCollisions(GameState& state);

        double friction;
        double thrust;
        double turnRate;
        double bulletSpeed;
        double asteroidSpeed;
        double baseAsteroidRadius;
        double bulletAge;
        double fireRate;
        double teleportCooldown;
        int maxBullets;
        int baseAsteroids;
        int width;
        int height;

};
#endif
