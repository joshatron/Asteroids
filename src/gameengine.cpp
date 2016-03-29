#include <stdlib.h>
#include "gameengine.h"
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
using glm::distance;

GameEngine::GameEngine()
{
    //these are best values found
    asteroidSpeed = 50;
    baseAsteroidRadius = 30;
    pauseTime = 3;
    deathTime = 1;
    baseAsteroids = 4;
    width = 1600;
    height = 900;
    srand(time(NULL));
}

void GameEngine::createInitialState(GameState& state)
{
    state.shipIndexes.push_back(0);
    state.ships.push_back(MainShip(vec2(width / 2, height / 2)));
    
    state.stats.lives = 3;
    state.stats.score = 0;

    //initialize asteroids
    state.nextNumAsteroids = baseAsteroids;
    createAsteroids(state, state.nextNumAsteroids);

    //wait to start game for 3 seconds
    state.pauseTime = pauseTime;
}

void GameEngine::updateShipControls(GameState& state, bool turningLeft, bool turningRight, bool thrusting, bool firing, bool teleporting, int ship)
{
    int index = state.shipIndexes.at(ship);
    if(index >= 0)
    {
        state.ships.at(index).turningLeft = turningLeft;
        state.ships.at(index).turningRight = turningRight;
        state.ships.at(index).thrusting = thrusting;
        state.ships.at(index).firing = firing;
        state.ships.at(index).teleporting = teleporting;
    }
}

void GameEngine::getNextState(GameState& state, double timePassed)
{
    updateObjects(state, timePassed);
    detectCollisions(state);
}

void GameEngine::updateObjects(GameState& state, double timePassed)
{
    if(state.stats.lives <= 0)
    {
        state.floatTime = 999;
    }
    state.pauseTime -= timePassed;

    //if we should be paused, exit
    if(state.pauseTime > 0)
    {
        return;
    }

    state.floatTime -= timePassed;

    if(state.floatTime <= 0 && state.shipIndexes.at(0) < 0)
    {
        vec2 cent = vec2(width / 2, height / 2);
        bool near = false;
        for(unsigned int k = 0; k < state.asteroids.size(); k++)
        {
            if(distance(state.asteroids.at(k).position, cent) < 200)
            {
                near = true;
                break;
            }
        }

        if(!near)
        {
            state.shipIndexes.at(0) = state.ships.size();
            state.ships.push_back(MainShip(cent));
        }
    }

    //update ship cooldowns
    for(unsigned int k = 0; k < state.ships.size(); k++)
    {
        state.ships.at(k).fireCooldown -= timePassed;
        state.ships.at(k).teleportCooldown -= timePassed;
        for(unsigned int a = 0; a < state.ships.at(k).bulletCooldowns.size(); a++)
        {
            state.ships.at(k).bulletCooldowns.at(a) -= timePassed;
            if(state.ships.at(k).bulletCooldowns.at(a) <= 0)
            {
                state.ships.at(k).bulletCooldowns.erase(state.ships.at(k).bulletCooldowns.begin() + a);
            }
        }
    }

    //update ship loctions
    for(unsigned int k = 0; k < state.ships.size(); k++)
    {
        updateLocation(state.ships.at(k).position, state.ships.at(k).velocity, timePassed);
    }

    //update asteroid locations
    for(unsigned int k = 0; k < state.asteroids.size(); k++)
    {
        updateLocation(state.asteroids.at(k).position, state.asteroids.at(k).velocity, timePassed);
    }

    //update and delete bullets
    for(unsigned int k = 0; k < state.bullets.size(); k++)
    {
        updateLocation(state.bullets.at(k).position, state.bullets.at(k).velocity, timePassed);
        state.bullets.at(k).age -= timePassed;
        if(state.bullets.at(k).age <= 0)
        {
            state.bullets.erase(state.bullets.begin() + k);
        }
    }

    //update and delete animations
    for(unsigned int k = 0; k < state.animations.size(); k++)
    {
        updateLocation(state.animations.at(k).position, state.bullets.at(k).velocity, timePassed);
        state.animations.at(k).angle += state.animations.at(k).turnRate * timePassed;
        state.animations.at(k).age -= timePassed;
        if(state.animations.at(k).age <= 0)
        {
            state.animations.erase(state.animations.begin() + k);
        }
    }

    //we ran out of asteroids... we need more
    if(state.asteroids.size() == 0)
    {
        state.nextNumAsteroids += 2;
        createAsteroids(state, state.nextNumAsteroids);
    }

    //update based on controls
    for(unsigned int k = 0; k < state.ships.size(); k++)
    {
        state.ships.at(k).updateVelocity(timePassed);
        //turning left
        if(state.ships.at(k).turningLeft && !state.ships.at(k).turningRight)
        {
            state.ships.at(k).angle -= state.ships.at(k).turnRate * timePassed;
            if(state.ships.at(k).angle < 0)
            {
                state.ships.at(k).angle += 2 * PI;
            }
        }
        //turning right
        else if(state.ships.at(k).turningRight && !state.ships.at(k).turningLeft)
        {
            state.ships.at(k).angle += state.ships.at(k).turnRate * timePassed;
            if(state.ships.at(k).angle > 2 * PI)
            {
                state.ships.at(k).angle -= 2 * PI;
            }
        }
        //firing
        if(state.ships.at(k).firing && state.ships.at(k).bulletCooldowns.size() < (unsigned int)state.ships.at(k).maxBullets && state.ships.at(k).fireCooldown <= 0)
        {
            state.bullets.push_back(state.ships.at(k).fire());

            state.ships.at(k).fireCooldown = state.ships.at(k).fireRate;
            state.ships.at(k).bulletCooldowns.push_back(state.ships.at(k).bulletAge);
        }
        //teleporting
        if(state.ships.at(k).teleporting && state.ships.at(k).teleportCooldown <= 0)
        {
            state.ships.at(k).position.x = abs(rand() % width);
            state.ships.at(k).position.y = abs(rand() % height);
            state.ships.at(k).velocity.x = 0;
            state.ships.at(k).velocity.y = 0;
            state.ships.at(k).teleportCooldown = state.ships.at(k).teleportRate;
        }
    }
}

void GameEngine::detectCollisions(GameState& state)
{
    //bullets and asteroids
    for(unsigned int k = 0; k < state.bullets.size(); k++)
    {
        for(unsigned int a = 0; a < state.asteroids.size(); a++)
        {
            if(CollisionDetection::shapeAndPoint(state.asteroids.at(a), state.bullets.at(k).position) >= 0)
            {
                state.bullets.erase(state.bullets.begin() + k);
                k--;
                
                destroyAsteroid(state, a);
                break;
            }
        }
    }

    //bullets and ships
    for(unsigned int k = 0; k < state.bullets.size(); k++)
    {
        for(unsigned int a = 0; a < state.ships.size(); a++)
        {
            if(CollisionDetection::shapeAndPoint(state.ships.at(a), state.bullets.at(k).position) >= 0)
            {
                state.bullets.erase(state.bullets.begin() + k);
                k--;
                
                destroyShip(state, a);
                break;
            }
        }
    }

    //ships and asteroids
    for(unsigned int k = 0; k < state.ships.size(); k++)
    {
        for(unsigned int a = 0; a < state.asteroids.size(); a++)
        {
            if(CollisionDetection::twoShapes(state.ships.at(k), state.asteroids.at(a)) >= 0)
            {
                destroyShip(state, k);
                k--;

                destroyAsteroid(state, a);
                break;
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
            for(unsigned int a = 0; a < state.ships.size(); a++)
            {
                if(distance(tempPoint, state.ships.at(a).position) < 200)
                {
                    done = false;
                    tempPoint.x = rand() % width;
                    tempPoint.y = rand() % height;
                    break;
                }
            }
        }

        createAsteroid(state, tempPoint, baseAsteroidRadius, asteroidSpeed);
    }
}

void GameEngine::createAsteroid(GameState& state, vec2 center, double radius, double velocity)
{
    ConvexShape currentShape;
    currentShape.points.push_back(vec2(0,0));

    //this method uses degrees instead of radians because it is easier to work with integers
    Asteroid asteroid;
    asteroid.position = center;
    asteroid.radius = radius;
    asteroid.angle = 0;
    double angle = abs(rand() % 360) * PI / 180;
    asteroid.velocity.x = velocity * cos(angle);
    asteroid.velocity.y = velocity * sin(angle);

    int last = -999;
    int sides = 7 + (abs(rand() % 5));
    double change = 360. / sides;
    angle = rand() % (int)(change / 2);
    angle = angle * PI / 180;
    vec2 first;
    for(int k = 0; k < sides; k++)
    {
        double tempRadius = radius;
        if(abs(rand()) % sides < 2 && last + 2 != k && last + 3 != k)
        {
            tempRadius = ((50 + abs(rand()) % 50) / 100.) * radius;
            last = k;
        }
        vec2 tempPoint = vec2(tempRadius * cos(angle), tempRadius * sin(angle));
        if(k == 0)
        {
            first.x = tempPoint.x;
            first.y = tempPoint.y;
            asteroid.points.push_back(tempPoint);
        }
        else
        {
            asteroid.points.push_back(tempPoint);
            asteroid.points.push_back(tempPoint);
        }

        currentShape.points.push_back(tempPoint);

        if(last == k)
        {
            asteroid.collisionShapes.push_back(currentShape);
            currentShape.points.clear();
            currentShape.points.push_back(vec2(0,0));
            currentShape.points.push_back(tempPoint);
        }

        angle = (k + 1) * change;
        angle += rand() % (int)(change / 2);
        angle = angle * PI / 180;

    }
    asteroid.points.push_back(first);

    if(currentShape.points.size() == asteroid.points.size() + 1)
    {
        currentShape.points.erase(currentShape.points.begin());
    }
    else
    {
        currentShape.points.push_back(asteroid.points.at(0));
    }
    asteroid.collisionShapes.push_back(currentShape);

    state.asteroids.push_back(asteroid);
}

void GameEngine::destroyAsteroid(GameState& state, int index)
{
    //if it was the largest one
    if(state.asteroids.at(index).radius + .1 > baseAsteroidRadius)
    {
        createAsteroid(state, state.asteroids.at(index).position, baseAsteroidRadius / 2, asteroidSpeed * 2);
        createAsteroid(state, state.asteroids.at(index).position, baseAsteroidRadius / 2, asteroidSpeed * 2);
        state.asteroids.erase(state.asteroids.begin() + index);
        addScore(state, 25);
        state.stats.largeAsteroidsDestroyed++;
    }
    //if it was a middle one
    else if(state.asteroids.at(index).radius + .1 > baseAsteroidRadius / 2)
    {
        createAsteroid(state, state.asteroids.at(index).position, baseAsteroidRadius / 3, asteroidSpeed * 3);
        createAsteroid(state, state.asteroids.at(index).position, baseAsteroidRadius / 3, asteroidSpeed * 3);
        state.asteroids.erase(state.asteroids.begin() + index);
        addScore(state, 50);
        state.stats.mediumAsteroidsDestroyed++;
    }
    //if it was the smallest one
    else
    {
        state.asteroids.erase(state.asteroids.begin() + index);
        addScore(state, 100);
        state.stats.smallAsteroidsDestroyed++;
    }

}

void GameEngine::destroyShip(GameState& state, int index)
{
    state.ships.erase(state.ships.begin() + index);
    //main ship died
    if(state.shipIndexes.at(0) == index)
    {
        state.floatTime = deathTime;
        state.shipIndexes.at(0) = -1;
        state.stats.lives--;

        //create animation
    }
}

void GameEngine::addScore(GameState& state, int points)
{
    int score = state.stats.score;
    if(score % 10000 > (score + points) % 10000)
    {
        state.stats.lives++;
    }
    state.stats.score += points;
}
