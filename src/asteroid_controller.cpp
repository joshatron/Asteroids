#include "asteroid_controller.h"
#include "main_ship.h"
#include "asteroid.h"
#include "hexa_ship.h"
#include "star.h"
#include <stdlib.h>
#include <iostream>

using std::make_shared;
using std::cout;
using std::endl;

AsteroidsController::AsteroidsController()
{
    srand(time(NULL));

    basePauseTime = 3;
    baseDeathTime = 1;
    baseAsteroids = 4;
    currentIndex = 1; 
    width = 1600;
    height = 900;

    counts.push_back(0);
    counts.push_back(0);
    counts.push_back(0);
    counts.push_back(0);
}

void AsteroidsController::initialize(GameState& state)
{
    lives = 3;
    score = 0;
    
    timeToReset = -1;
    floatTime = -1;
    pauseTime = basePauseTime;

    state.asteroidSpeed = 50;
    state.baseAsteroidRadius = 30;
    state.gravityConstant = 1;
    state.width = 1600;
    state.height = 900;

    state.objects.push_back(make_shared<MainShip>(vec2(width / 6, height / 6), 1));
    counts.at(1) = 1;

    /*state.objects.push_back(make_shared<HexaShip>(vec2(width / 6, height / 6), 3));
    counts.at(3) = 1;*/

    state.objects.push_back(make_shared<Star>(vec2(width / 2., height / 2.), vec2(0,0), 100, 100000., 10.));

    state.nextNumAsteroids = baseAsteroids;
    createAsteroids(state, state.nextNumAsteroids);
    counts.at(2) = state.nextNumAsteroids;
}

void AsteroidsController::updateCount(GameState& state, int index, bool add)
{
    if(add)
    {
        counts.at(index) += 1;
    }
    else
    {
        counts.at(index) -= 1;
        if(index == 2)
        {
            updateScore(100);
        }
    }


    if(index == 1 && counts.at(1) == 0)
    {
        timeToReset = baseDeathTime;
        lives--;
    }

    if(index == 2 && counts.at(2) == 0)
    {
        state.nextNumAsteroids += 2;
        createAsteroids(state, state.nextNumAsteroids);
        counts.at(2) = state.nextNumAsteroids;
    }
}

void AsteroidsController::updateTimers(GameState& state, double timePassed)
{
    timeToReset -= timePassed;
    floatTime -= timePassed;
    pauseTime -= timePassed;

    if(counts.at(1) == 0 && timeToReset <= 0 && lives > 0)
    {
        vec2 cent = vec2(width / 6, height / 6);
        bool near = false;
        for(unsigned int k = 0; k < state.objects.size(); k++)
        {
            if(distance(state.objects.at(k)->position, cent) < 150)
            {
                near = true;
                break;
            }
        }
        
        if(!near)
        {
            state.objects.push_back(make_shared<MainShip>(vec2(width / 6, height / 6), 1));
            counts.at(1) = 1;
        }
    }
}

void AsteroidsController::createAsteroids(GameState& state, int number)
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
                if(distance(tempPoint, state.objects.at(a)->position) < 150)
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

void AsteroidsController::updateScore(int points)
{
    if(score % 10000 > (score + points) % 10000)
    {
        lives++;
    }
    score += points;
}
