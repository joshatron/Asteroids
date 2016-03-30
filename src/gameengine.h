#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <glm/glm.hpp>
#include "gamestate.h"

using glm::vec2;

class GameEngine
{
    public:
        GameEngine();
        void createInitialState(GameState& state);
        void getNextState(GameState& state, double timePassed);
    private:
        void createMainShip(GameState& state, vec2 location);
        void createAsteroids(GameState& state, int number);
        void createAsteroid(GameState& state, vec2 center, double radius, double velocity);
        void updateObjectLocations(GameState& state, double timePassed);
        void updateObjects(GameState& state, double timePassed);
        void detectCollisions(GameState& state);
        void updateLocation(vec2& original, vec2& velocity, double time);
        void destroyAsteroid(GameState& state, int index);
        void destroyShip(GameState& state, int index);
        void addScore(GameState& state, int points);

        double asteroidSpeed;
        double baseAsteroidRadius;
        double pauseTime;
        double deathTime;
        double gravityConstant;
        int baseAsteroids;
        int width;
        int height;

};
#endif
