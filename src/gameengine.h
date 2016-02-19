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
        GameState * createInitialState();
        void getNextState(GameState *previous, double timePassed);
        Asteroid * createAsteroid(vec2 *center, double radius, double velocity);
    private:
        vector<Asteroid> * createAsteroids(int number, vec2 *shipLoc);
        void updateLocation(vec2 *original, vec2 *velocity, double time);
        void updateVelocity(vec2 *velocity, double angle, double add);

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
        int width;
        int height;

};
#endif
