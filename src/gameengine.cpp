#include "gameengine.h"
#include <stdlib.h>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include "collision.h"
#include "convex_shape.h"
#include "main_ship.h"
#include "asteroid.h"
#include "ship.h"
#include "bullet.h"
#include "star.h"
#include "main_ship.h"
#include "animation.h"

#ifndef PI
    #define PI 3.14159265
#endif

using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;
using glm::distance;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::rotate;

GameEngine::GameEngine()
{
    //these are best values found
    srand(time(NULL));
}

void GameEngine::createInitialState(GameState& state)
{
    controller.initialize(state);
}

void GameEngine::getNextState(GameState& state, double timePassed)
{
    updateObjectLocations(state, timePassed);
    detectCollisions(state);
    updateObjects(state, timePassed);
}

void GameEngine::updateObjectLocations(GameState& state, double timePassed)
{
    controller.updateTimers(state, timePassed);
    //if we should be paused, exit
    if(controller.pauseTime > 0)
    {
        return;
    }

    for(unsigned int k = 0; k < state.objects.size(); k++)
    {
        shared_ptr<Object> object = state.objects.at(k);
        object->updateTimers(timePassed);
        updateLocation(object->position, object->velocity, timePassed);
        //destroy
        if(object->timeToLive <= 0)
        {
            vector<shared_ptr<Object>> newOnes = object->destroy(state, -1);

            for(unsigned int a = 0; a < newOnes.size(); a++)
            {
                controller.updateCount(state, newOnes.at(a)->typeIndex, true);
                state.objects.push_back(newOnes.at(a));
            }

            controller.updateCount(state, state.objects.at(k)->typeIndex, false);
            state.objects.erase(state.objects.begin() + k);
        }
        //apply gravity
        else if(object->mass > .00001)
        {
            for(unsigned int a = 0; a < state.objects.size(); a++)
            {
                if(a != k)
                {
                    shared_ptr<Object> object2 = state.objects.at(a);
                    double acceleration = state.gravityConstant * (object->mass / pow(distance(object->position, object2->position), 2));
                    double angle = atan2(object->position.y - object2->position.y, object->position.x - object2->position.x);
                    object2->velocity.x += cos(angle) * acceleration;
                    object2->velocity.y += sin(angle) * acceleration;
                }
            }
        }
    }
}

void GameEngine::updateObjects(GameState& state, double timePassed)
{
    if(controller.pauseTime <= 0 && controller.floatTime <= 0)
    {
        for(unsigned int k = 0; k < state.objects.size(); k++)
        {
            vector<shared_ptr<Object>> newOnes = state.objects.at(k)->updateFromControls(state, timePassed);
            state.objects.insert(state.objects.end(), newOnes.begin(), newOnes.end());
        }
    }
}

void GameEngine::detectCollisions(GameState& state)
{
    for(unsigned int k = 0; k < state.objects.size() - 1; k++)
    {
        if(state.objects.at(k)->collisionIndex != -1)
        {
            for(unsigned int a = k + 1; a < state.objects.size(); a++)
            {
                if(state.objects.at(k)->collisionIndex != state.objects.at(a)->collisionIndex && state.objects.at(a)->collisionIndex != -1)
                {
                    int region = CollisionDetection::twoShapes(state.objects.at(k), state.objects.at(a));
                    if(region >= 0)
                    {
                        int region2 = CollisionDetection::twoShapes(state.objects.at(a), state.objects.at(k));

                        vector<shared_ptr<Object>> newOnes = state.objects.at(k)->destroy(state, region);

                        for(unsigned int a = 0; a < newOnes.size(); a++)
                        {
                            controller.updateCount(state, newOnes.at(a)->typeIndex, true);
                            state.objects.push_back(newOnes.at(a));
                        }

                        controller.updateCount(state, state.objects.at(k)->typeIndex, false);
                        state.objects.erase(state.objects.begin() + k);

                        k--;
                        a--;

                        vector<shared_ptr<Object>> newOnes2 = state.objects.at(a)->destroy(state, region2);

                        for(unsigned int a = 0; a < newOnes2.size(); a++)
                        {
                            controller.updateCount(state, newOnes2.at(a)->typeIndex, true);
                            state.objects.push_back(newOnes2.at(a));
                        }

                        controller.updateCount(state, state.objects.at(k)->typeIndex, false);
                        state.objects.erase(state.objects.begin() + k);

                        break;
                    }
                }
            }
        }
    }
}

void GameEngine::updateLocation(vec2& original, vec2& velocity, double time)
{
    original.x += (velocity.x * time);
    if(original.x > width)
    {
        original.x = original.x - width;
    }
    else if(original.x < 0)
    {
        original.x = original.x + width;
    }

    original.y += (velocity.y * time);
    if(original.y > height)
    {
        original.y = original.y - height;
    }
    else if(original.y < 0)
    {
        original.y = original.y + height;
    }
}

void GameEngine::addScore(GameState& state, int points)
{
    int score = state.stats->score;
    if(score % 10000 > (score + points) % 10000)
    {
        state.stats->lives++;
    }
    state.stats->score += points;
}
