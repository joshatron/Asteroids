#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <glm/glm.hpp>
#include "gamestate.h"
#include "bullet.h"
#include "asteroid.h"
#include "ship.h"
#include "animation.h"

using glm::vec2;
using std::vector;

class GameEngine
{
    public:
        GameEngine();
        void createInitialState(GameState& state);
        void getNextState(GameState& state, double timePassed);
        void updateShipControls(GameState& state, bool turningLeft, bool turningRight, bool thrusting, bool firing, bool teleporting, int ship);
    private:
        void createMainShip(GameState& state, vec2 location);
        void createAsteroids(GameState& state, int number);
        void createAsteroid(GameState& state, vec2 center, double radius, double velocity);
        void updateObjects(GameState& state, double timePassed);
        void detectCollisions(GameState& state);
        void updateLocation(vec2& original, vec2& velocity, double time);
        void updateVelocity(vec2& velocity, double angle, double add);

        double friction;
        double thrust;
        double turnRate;
        double bulletSpeed;
        double asteroidSpeed;
        double baseAsteroidRadius;
        double bulletAge;
        double fireRate;
        double teleportCooldown;
        double pauseTime;
        double deathTime;
        int maxBullets;
        int baseAsteroids;
        int width;
        int height;

};
#endif
