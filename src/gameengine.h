#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#define GLM_FORCE_RADIANS
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
        void updateObjectLocations(GameState& state, double timePassed);
        void updateObjects(GameState& state, double timePassed);
        void detectCollisions(GameState& state);
        void updateLocation(vec2& original, vec2& velocity, double time);

        Controller controller;
};
#endif
