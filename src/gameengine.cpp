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
    currentIndex = 1;
    basePauseTime = 3;
    baseDeathTime = 1;
    baseAsteroids = 4;
    width = 1600;
    height = 900;
    srand(time(NULL));
}

void GameEngine::createInitialState(GameState& state)
{
    state.width = width;
    state.height = height;
    state.asteroidSpeed = 50;
    state.baseAsteroidRadius = 30;
    state.gravityConstant = 1;
    state.objects.push_back(make_shared<MainShip>(vec2(width / 2, height / 2), 1));

    /*state.stars.push_back(make_shared<Star>());
    state.stars.at(0)->position.x = width / 2;
    state.stars.at(0)->position.y = height / 2;
    state.stars.at(0)->gravity = 1000;*/
    
    shared_ptr<Stats> newStats(make_shared<Stats>());
    state.stats = newStats;
    state.stats->lives = 3;
    state.stats->score = 0;
    state.stats->mainShipIndex = 0;

    //initialize asteroids
    state.nextNumAsteroids = baseAsteroids;
    createAsteroids(state, state.nextNumAsteroids);

    //wait to start game for 3 seconds
    state.pauseTime = basePauseTime;
}

void GameEngine::getNextState(GameState& state, double timePassed)
{
    updateObjectLocations(state, timePassed);
    detectCollisions(state);
    updateObjects(state, timePassed);
}

void GameEngine::updateObjectLocations(GameState& state, double timePassed)
{
    if(state.stats->lives <= 0)
    {
        state.floatTime = 999;
    }
    state.pauseTime -= timePassed;
    state.stats->timeToReset -= timePassed;

    //if we should be paused, exit
    if(state.pauseTime > 0)
    {
        return;
    }

    state.floatTime -= timePassed;

    //reset (for after a death)
    /*if(state.stats->timeToReset <= 0 && state.stats->mainShipIndex == -1 && state.stats->lives > 0)
    {
        vec2 cent = vec2(width / 2, height / 2);
        bool near = false;
        for(unsigned int k = 0; k < state.asteroids.size(); k++)
        {
            if(distance(state.asteroids.at(k)->position, cent) < 150)
            {
                near = true;
                break;
            }
        }

        if(!near)
        {
            state.stats->mainShipIndex = state.ships.size();
            state.ships.push_back(make_shared<MainShip>(cent));
        }
    }*/

    //we ran out of asteroids... we need more
    /*if(state.asteroids.size() == 0)
    {
        state.nextNumAsteroids += 2;
        createAsteroids(state, state.nextNumAsteroids);
    }*/

    for(unsigned int k = 0; k < state.objects.size(); k++)
    {
        shared_ptr<Object> object = state.objects.at(k);
        object->updateTimers(timePassed);
        updateLocation(object->position, object->velocity, timePassed);
        //destroy
        if(object->timeToLive <= 0)
        {
            vector<shared_ptr<Object>> newOnes = object->destroy(state, -1);
            state.objects.insert(state.objects.end(), newOnes.begin(), newOnes.end());
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
    for(unsigned int k = 0; k < state.objects.size(); k++)
    {
        vector<shared_ptr<Object>> newOnes = state.objects.at(k)->updateFromControls(state, timePassed);
        state.objects.insert(state.objects.end(), newOnes.begin(), newOnes.end());
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
                        state.objects.insert(state.objects.end(), newOnes.begin(), newOnes.end());
                        state.objects.erase(state.objects.begin() + k);
                        k--;
                        a--;

                        vector<shared_ptr<Object>> newOnes2 = state.objects.at(a)->destroy(state, region2);
                        state.objects.insert(state.objects.end(), newOnes2.begin(), newOnes2.end());
                        state.objects.erase(state.objects.begin() + a);
                        a--;
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

void GameEngine::createAsteroids(GameState& state, int number)
{
    for(int k = 0; k < number; k++)
    {
        vec2 tempPoint = vec2(rand() % width, rand() % height);
        bool done = false;
        while(!done)
        {
            done = true;
            for(unsigned int a = 0; a < state.objects.size(); a++)
            {
                if(distance(tempPoint, state.objects.at(a)->position) < 200)
                {
                    done = false;
                    tempPoint.x = rand() % width;
                    tempPoint.y = rand() % height;
                    break;
                }
            }
        }

        shared_ptr<Object> asteroid(make_shared<Asteroid>(tempPoint, state.baseAsteroidRadius, state.asteroidSpeed));
        state.objects.push_back(asteroid);
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
