#include <stdlib.h>
#include <memory>
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
using std::make_shared;
using std::shared_ptr;
using glm::distance;

GameEngine::GameEngine()
{
    //these are best values found
    asteroidSpeed = 50;
    baseAsteroidRadius = 30;
    pauseTime = 3;
    deathTime = 1;
    gravityConstant = 1;
    baseAsteroids = 4;
    width = 1600;
    height = 900;
    srand(time(NULL));
}

void GameEngine::createInitialState(GameState& state)
{
    state.ships.push_back(make_shared<MainShip>(vec2(width / 2, height / 2)));

    /*state.stars.push_back(make_shared<Star>());
    state.stars.at(0)->position.x = width / 2;
    state.stars.at(0)->position.y = height / 2;
    state.stars.at(0)->gravity = 2000;*/
    
    shared_ptr<Stats> newStats(make_shared<Stats>());
    state.stats = newStats;
    state.stats->lives = 3;
    state.stats->score = 0;
    state.stats->mainShipIndex = 0;

    //initialize asteroids
    state.nextNumAsteroids = baseAsteroids;
    createAsteroids(state, state.nextNumAsteroids);

    //wait to start game for 3 seconds
    state.pauseTime = pauseTime;
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

    if(state.stats->timeToReset <= 0 && state.stats->mainShipIndex == -1 && state.stats->lives > 0)
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
    }

    //update ships
    for(unsigned int k = 0; k < state.ships.size(); k++)
    {
        shared_ptr<Ship> ship = state.ships.at(k);
        ship->fireCooldown -= timePassed;
        ship->teleportCooldown -= timePassed;
        for(unsigned int a = 0; a < ship->bulletCooldowns.size(); a++)
        {
            ship->bulletCooldowns.at(a) -= timePassed;
            if(ship->bulletCooldowns.at(a) <= 0)
            {
                ship->bulletCooldowns.erase(ship->bulletCooldowns.begin() + a);
            }
        }

        updateLocation(ship->position, ship->velocity, timePassed);
    }

    //update asteroid locations
    for(unsigned int k = 0; k < state.asteroids.size(); k++)
    {
        updateLocation(state.asteroids.at(k)->position, state.asteroids.at(k)->velocity, timePassed);
    }

    //update and delete bullets
    for(unsigned int k = 0; k < state.bullets.size(); k++)
    {
        updateLocation(state.bullets.at(k)->position, state.bullets.at(k)->velocity, timePassed);
        state.bullets.at(k)->age -= timePassed;
        if(state.bullets.at(k)->age <= 0)
        {
            state.bullets.erase(state.bullets.begin() + k);
        }
    }

    //update and delete animations
    for(unsigned int k = 0; k < state.animations.size(); k++)
    {
        shared_ptr<Animation> animation = state.animations.at(k);
        updateLocation(animation->position, animation->velocity, timePassed);
        animation->angle += animation->turnRate * timePassed;
        animation->age -= timePassed;
        if(animation->age <= 0)
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

    for(unsigned int a = 0; a < state.stars.size(); a++)
    {
        shared_ptr<Star> star = state.stars.at(a);
        //update ship velocities based on gravities
        for(unsigned int k = 0; k < state.ships.size(); k++)
        {
            shared_ptr<Ship> ship = state.ships.at(k);
            double acceleration = gravityConstant * (star->gravity / distance(star->position, ship->position));
            double angle = atan2(star->position.y - ship->position.y, star->position.x - ship->position.x);
            ship->velocity.x += cos(angle) * acceleration;
            ship->velocity.y += sin(angle) * acceleration;
        }

        //update bullet velocities based on gravities
        for(unsigned int k = 0; k < state.bullets.size(); k++)
        {
            shared_ptr<Bullet> bullet = state.bullets.at(k);
            double acceleration = gravityConstant * (star->gravity / distance(star->position, bullet->position));
            double angle = atan2(star->position.y - bullet->position.y, star->position.x - bullet->position.x);
            bullet->velocity.x += cos(angle) * acceleration;
            bullet->velocity.y += sin(angle) * acceleration;
        }
    }
}

void GameEngine::updateObjects(GameState& state, double timePassed)
{
    //update based on controls
    for(unsigned int k = 0; k < state.ships.size(); k++)
    {
        shared_ptr<Ship> ship = state.ships.at(k);
        //turning left
        if(ship->turningLeft && !ship->turningRight)
        {
            ship->angle -= ship->turnRate * timePassed;
            if(ship->angle < 0)
            {
                ship->angle += 2 * PI;
            }
        }
        //turning right
        else if(ship->turningRight && !ship->turningLeft)
        {
            ship->angle += ship->turnRate * timePassed;
            if(ship->angle > 2 * PI)
            {
                ship->angle -= 2 * PI;
            }
        }
        if(ship->thrusting)
        {
            ship->velocity.x += cos(ship->angle - (PI / 2)) * (ship->thrust * timePassed);
            ship->velocity.y += sin(ship->angle - (PI / 2)) * (ship->thrust * timePassed);
        }
        else
        {
            double totalVel = distance(ship->velocity, vec2(0,0));
            if(ship->velocity.x != 0)
            {
                ship->velocity.x -= (ship->velocity.x / totalVel) * ship->friction * timePassed;
            }
            if(ship->velocity.y != 0)
            {
                ship->velocity.y -= (ship->velocity.y / totalVel) * ship->friction * timePassed;
            }
        }
        //firing
        if(ship->firing && ship->bulletCooldowns.size() < (unsigned int)ship->maxBullets && ship->fireCooldown <= 0)
        {
            for(unsigned int a = 0; a < ship->bulletFirePoints.size(); a++)
            {
                double dist = distance(ship->bulletFirePoints.at(a), vec2(0,0));
                double angle = atan(ship->bulletFirePoints.at(a).y / ship->bulletFirePoints.at(a).x);
                shared_ptr<Bullet> bullet(make_shared<Bullet>());
                bullet->position.x = ship->position.x + (dist * cos(ship->angle + angle));
                bullet->position.y = ship->position.y + (dist * sin(ship->angle + angle));
                bullet->velocity.x = cos(ship->angle - (PI / 2)) * ship->bulletSpeed + ship->velocity.x;
                bullet->velocity.y = sin(ship->angle - (PI / 2)) * ship->bulletSpeed + ship->velocity.y;
                bullet->age = ship->bulletAge;
                state.bullets.push_back(bullet);
            }

            ship->fireCooldown = ship->fireRate;
            ship->bulletCooldowns.push_back(ship->bulletAge);
        }
        //teleporting
        if(ship->teleporting && ship->teleportCooldown <= 0)
        {
            ship->position.x = abs(rand() % width);
            ship->position.y = abs(rand() % height);
            ship->velocity.x = 0;
            ship->velocity.y = 0;
            ship->teleportCooldown = ship->teleportRate;
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
            if(CollisionDetection::shapeAndPoint(state.asteroids.at(a), state.bullets.at(k)->position) >= 0)
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
            if(CollisionDetection::shapeAndPoint(state.ships.at(a), state.bullets.at(k)->position) >= 0)
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
                if(distance(tempPoint, state.ships.at(a)->position) < 200)
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
    shared_ptr<Asteroid> asteroid(make_shared<Asteroid>());
    asteroid->position = center;
    asteroid->radius = radius;
    asteroid->angle = 0;
    double angle = abs(rand() % 360) * PI / 180;
    asteroid->velocity.x = velocity * cos(angle);
    asteroid->velocity.y = velocity * sin(angle);

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
            asteroid->points.push_back(tempPoint);
        }
        else
        {
            asteroid->points.push_back(tempPoint);
            asteroid->points.push_back(tempPoint);
        }

        currentShape.points.push_back(tempPoint);

        if(last == k)
        {
            asteroid->collisionShapes.push_back(currentShape);
            currentShape.points.clear();
            currentShape.points.push_back(vec2(0,0));
            currentShape.points.push_back(tempPoint);
        }

        angle = (k + 1) * change;
        angle += rand() % (int)(change / 2);
        angle = angle * PI / 180;

    }
    asteroid->points.push_back(first);

    if(currentShape.points.size() == asteroid->points.size() + 1)
    {
        currentShape.points.erase(currentShape.points.begin());
    }
    else
    {
        currentShape.points.push_back(asteroid->points.at(0));
    }
    asteroid->collisionShapes.push_back(currentShape);

    state.asteroids.push_back(asteroid);
}

void GameEngine::destroyAsteroid(GameState& state, int index)
{
    shared_ptr<Asteroid> asteroid = state.asteroids.at(index);
    //if it was the largest one
    if(asteroid->radius + .1 > baseAsteroidRadius)
    {
        createAsteroid(state, asteroid->position, baseAsteroidRadius / 2, asteroidSpeed * 2);
        createAsteroid(state, asteroid->position, baseAsteroidRadius / 2, asteroidSpeed * 2);
        state.asteroids.erase(state.asteroids.begin() + index);
        addScore(state, 25);
        state.stats->largeAsteroidsDestroyed++;
    }
    //if it was a middle one
    else if(asteroid->radius + .1 > baseAsteroidRadius / 2)
    {
        createAsteroid(state, asteroid->position, baseAsteroidRadius / 3, asteroidSpeed * 3);
        createAsteroid(state, asteroid->position, baseAsteroidRadius / 3, asteroidSpeed * 3);
        state.asteroids.erase(state.asteroids.begin() + index);
        addScore(state, 50);
        state.stats->mediumAsteroidsDestroyed++;
    }
    //if it was the smallest one
    else
    {
        state.asteroids.erase(state.asteroids.begin() + index);
        addScore(state, 100);
        state.stats->smallAsteroidsDestroyed++;
    }
}

void GameEngine::destroyShip(GameState& state, int index)
{
    state.ships.erase(state.ships.begin() + index);
    //main ship died
    if(state.stats->mainShipIndex == index)
    {
        state.stats->timeToReset = deathTime;
        state.stats->mainShipIndex = -1;
        state.stats->lives--;

        //create animation
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
