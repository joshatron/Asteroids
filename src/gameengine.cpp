#include <stdlib.h>
#include "gameengine.h"
#include <iostream>
#include <cmath>

#define PI 3.14159265

using std::cout;
using std::endl;
using glm::distance;

GameEngine::GameEngine()
{
    //these are best values found
    friction = 40;
    thrust = 300;
    turnRate = 300;
    bulletSpeed = 250;
    asteroidSpeed = 50;
    baseAsteroidRadius = 30;
    bulletAge = 3;
    fireRate = .15;
    teleportCooldown = .5;
    pauseTime = 1;
    deathTime = 1;
    maxBullets = 4;
    baseAsteroids = 4;
    width = 1600;
    height = 900;
    srand(time(NULL));
}

void GameEngine::createInitialState(GameState& state)
{
    createMainShip(state, vec2(width / 2, height / 2));

    //initialize asteroids
    state.nextNumAsteroids = baseAsteroids;
    createAsteroids(state, state.nextNumAsteroids);

    //wait to start game for 3 seconds
    state.pauseTime = pauseTime;
}

void GameEngine::getNextState(GameState& state, double timePassed)
{
    updateObjects(state, timePassed);
    detectCollisions(state);
}

void GameEngine::updateObjects(GameState& state, double timePassed)
{
    state.pauseTime -= timePassed;

    //if we should be paused, exit
    if(state.pauseTime > 0)
    {
        return;
    }

    state.playTime -= timePassed;

    if(state.playTime <= 0 && state.mainShipIndex < 0)
    {
        createMainShip(state, vec2(width / 2, height / 2));
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
        //turning left
        if(state.ships.at(k).turningLeft && !state.ships.at(k).turningRight)
        {
            state.ships.at(k).angle -= turnRate * timePassed;
            if(state.ships.at(k).angle < 0)
            {
                state.ships.at(k).angle += 360;
            }
        }
        //turning right
        else if(state.ships.at(k).turningRight && !state.ships.at(k).turningLeft)
        {
            state.ships.at(k).angle += turnRate * timePassed;
            if(state.ships.at(k).angle > 360)
            {
                state.ships.at(k).angle -= 360;
            }
        }
        //thrusting
        if(state.ships.at(k).thrusting)
        {
            updateVelocity(state.ships.at(k).velocity, state.ships.at(k).angle, thrust * timePassed);
        }
        //friction
        else
        {
            double totalVel = distance(state.ships.at(k).velocity, vec2(0,0));
            if(state.ships.at(k).velocity.x != 0)
            {
                state.ships.at(k).velocity.x -= (state.ships.at(k).velocity.x / totalVel) * friction * timePassed;
            }
            if(state.ships.at(k).velocity.y != 0)
            {
                state.ships.at(k).velocity.y -= (state.ships.at(k).velocity.y / totalVel) * friction * timePassed;
            }
        }
        //firing
        if(state.ships.at(k).firing && state.ships.at(k).bulletCooldowns.size() < (unsigned int)maxBullets && state.ships.at(k).fireCooldown <= 0)
        {
            double dist = distance(state.ships.at(k).bulletFirePoint, vec2(0,0));
            double angle = atan(state.ships.at(k).bulletFirePoint.y / state.ships.at(k).bulletFirePoint.x);
            Bullet bullet;
            bullet.position.x = state.ships.at(k).position.x + (dist * cos((state.ships.at(k).angle) * PI / 180 + angle));
            bullet.position.y = state.ships.at(k).position.y + (dist * sin((state.ships.at(k).angle) * PI / 180 + angle));
            bullet.velocity.x = cos((state.ships.at(k).angle - 90) * PI / 180) * bulletSpeed + state.ships.at(k).velocity.x;
            bullet.velocity.y = sin((state.ships.at(k).angle - 90) * PI / 180) * bulletSpeed + state.ships.at(k).velocity.y;
            bullet.age = bulletAge;
            state.bullets.push_back(bullet);

            state.ships.at(k).fireCooldown = fireRate;
            state.ships.at(k).bulletCooldowns.push_back(bulletAge);
        }
        //teleporting
        if(state.ships.at(k).teleporting && state.ships.at(k).teleportCooldown <= 0)
        {
            state.ships.at(k).position.x = abs(rand() % width);
            state.ships.at(k).position.y = abs(rand() % height);
            state.ships.at(k).velocity.x = 0;
            state.ships.at(k).velocity.y = 0;
            state.ships.at(k).teleportCooldown = teleportCooldown;
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
            //we are inside the asteroid radius
            if(distance(state.bullets.at(k).position, state.asteroids.at(a).position) < state.asteroids.at(a).radius)
            {
                state.bullets.erase(state.bullets.begin() + k);
                
                //if it was the largest one
                if(state.asteroids.at(a).radius + .1 > baseAsteroidRadius)
                {
                    createAsteroid(state, state.asteroids.at(a).position, baseAsteroidRadius / 2, asteroidSpeed * 2);
                    createAsteroid(state, state.asteroids.at(a).position, baseAsteroidRadius / 2, asteroidSpeed * 2);
                    state.asteroids.erase(state.asteroids.begin() + a);
                    //this is so we don't skip the next asteroid
                    a--;
                }
                //if it was a middle one
                else if(state.asteroids.at(a).radius + .1 > baseAsteroidRadius / 2)
                {
                    createAsteroid(state, state.asteroids.at(a).position, baseAsteroidRadius / 3, asteroidSpeed * 3);
                    createAsteroid(state, state.asteroids.at(a).position, baseAsteroidRadius / 3, asteroidSpeed * 3);
                    state.asteroids.erase(state.asteroids.begin() + a);
                    //this is so we don't skip the next asteroid
                    a--;
                }
                //if it was the smallest one
                else
                {
                    state.asteroids.erase(state.asteroids.begin() + a);
                    //this is so we don't skip the next asteroid
                    a--;
                }

                break;
            }
        }
    }

    //ships and asteroids
    int ships = state.ships.size();
    for(int k = 0; k < ships; k++)
    {
        bool deleted = false;
        for(unsigned int t = 0; !deleted && t < state.ships.at(k).shipPoints.size(); t += 2)
        {
            for(unsigned int a = 0; a < state.asteroids.size(); a++)
            {
                if(distance(state.ships.at(k).shipPoints.at(t) + state.ships.at(k).position, state.asteroids.at(a).position) < state.asteroids.at(a).radius)
                {
                    //move ship offscreen so engine knows to put us back at start when clear
                    state.ships.erase(state.ships.begin() + k);
                    //main ship died
                    if(state.mainShipIndex == k)
                    {
                        state.playTime = deathTime;
                        state.mainShipIndex = -1;

                        //create animation
                    }
                    k--;
                    ships--;
                    deleted = true;

                    //if it was the largest one
                    if(state.asteroids.at(a).radius + .1 > baseAsteroidRadius)
                    {
                        createAsteroid(state, state.asteroids.at(a).position, baseAsteroidRadius / 2, asteroidSpeed * 2);
                        createAsteroid(state, state.asteroids.at(a).position, baseAsteroidRadius / 2, asteroidSpeed * 2);
                        state.asteroids.erase(state.asteroids.begin() + a);
                        //this is so we don't skip the next asteroid
                        a--;
                    }
                    //if it was a middle one
                    else if(state.asteroids.at(a).radius + .1 > baseAsteroidRadius / 2)
                    {
                        createAsteroid(state, state.asteroids.at(a).position, baseAsteroidRadius / 3, asteroidSpeed * 3);
                        createAsteroid(state, state.asteroids.at(a).position, baseAsteroidRadius / 3, asteroidSpeed * 3);
                        state.asteroids.erase(state.asteroids.begin() + a);
                        //this is so we don't skip the next asteroid
                        a--;
                    }
                    //if it was the smallest one
                    else
                    {
                        state.asteroids.erase(state.asteroids.begin() + a);
                        //this is so we don't skip the next asteroid
                        a--;
                    }

                    break;
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

void GameEngine::updateVelocity(vec2& velocity, double angle, double add)
{
    velocity.x += cos((angle - 90) * PI / 180) * add;
    velocity.y += sin((angle - 90) * PI / 180) * add;
}

void GameEngine::createMainShip(GameState& state, vec2 location)
{
    Ship mainShip;
    mainShip.position.x = location.x;
    mainShip.position.y = location.y;
    mainShip.velocity.x = 0;
    mainShip.velocity.y = 0;
    mainShip.angle = 0;
    mainShip.fireCooldown = 0;
    mainShip.teleportCooldown = 0;
    mainShip.lives = 3;
    mainShip.score = 0;
    mainShip.turningLeft = false;
    mainShip.turningRight = false;
    mainShip.thrusting = false;
    mainShip.firing = false;
    mainShip.teleporting = false;
    mainShip.shipPoints.push_back(vec2(0, -15));
    mainShip.shipPoints.push_back(vec2(8, 15));
    mainShip.shipPoints.push_back(vec2(8, 15));
    mainShip.shipPoints.push_back(vec2(4, 10));
    mainShip.shipPoints.push_back(vec2(4, 10));
    mainShip.shipPoints.push_back(vec2(-4, 10));
    mainShip.shipPoints.push_back(vec2(-4, 10));
    mainShip.shipPoints.push_back(vec2(-8, 15));
    mainShip.shipPoints.push_back(vec2(-8, 15));
    mainShip.shipPoints.push_back(vec2(0, -15));
    mainShip.shipFirePoints.push_back(vec2(4, 10));
    mainShip.shipFirePoints.push_back(vec2(0, 20));
    mainShip.shipFirePoints.push_back(vec2(0, 20));
    mainShip.shipFirePoints.push_back(vec2(-4, 10));
    mainShip.bulletFirePoint.x = 0;
    mainShip.bulletFirePoint.y = -15;

    state.mainShipIndex = state.ships.size();
    state.ships.push_back(mainShip);
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
    Asteroid asteroid;
    asteroid.position = center;
    asteroid.radius = radius;
    double angle = abs(rand() % 360) * PI / 180;
    asteroid.velocity.x = velocity * cos(angle);
    asteroid.velocity.y = velocity * sin(angle);
    
    int last = -999;
    int sides = 7 + (abs(rand() % 5));
    double change = 360. / sides;
    angle = rand() % (int)(change / 2);
    angle = angle * PI / 180;
    for(int k = 0; k < sides; k++)
    {
        double tempRadius = radius;
        if(abs(rand()) % sides < 2 && last + 2 != k && last + 3 != k)
        {
            tempRadius = ((50 + abs(rand()) % 50) / 100.) * radius;
            last = k;
        }
        vec2 tempPoint = vec2(tempRadius * cos(angle), tempRadius * sin(angle));
        asteroid.points.push_back(tempPoint);
        angle = (k + 1) * change;
        angle += rand() % (int)(change / 2);
        angle = angle * PI / 180;
    }

    state.asteroids.push_back(asteroid);
}
