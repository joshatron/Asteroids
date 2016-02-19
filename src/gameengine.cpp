#include <stdlib.h>
#include "gameengine.h"
#include <iostream>
#include <cmath>

#define PI 3.14159265

GameEngine::GameEngine()
{
    friction = 40;
    thrust = 200;
    turnRate = 200;
    bulletSpeed = 250;
    asteroidSpeed = 50;
    baseAsteroidRadius = 30;
    bulletAge = 2;
    fireRate = .15;
    teleportCooldown = .5;
    maxBullets = 4;
    width = 640;
    height = 480;
    srand(time(NULL));
}

GameState * GameEngine::createInitialState()
{
    GameState *state = new GameState();

    state->shipLoc = new vec2(width / 2, height / 2);
    state->shipVelocity = new vec2(0, 0);
    state->shipAngle = 0.;
    state->livesLeft = 3;
    state->score = 0;
    state->bullets = new vector<Bullet>();
    state->asteroids = createAsteroids(10, state->shipLoc);
    state->pauseTime = 1.;
    state->lastFiredCooldown = -1.;
    state->teleportCooldown = -1.;
    state->turningLeft = false;
    state->turningRight = false;
    state->thrusting = false;
    state->firing = false;
    state->teleporting = false;

    return state;
}

void GameEngine::getNextState(GameState *previous, double timePassed)
{
    previous->lastFiredCooldown -= timePassed;
    previous->teleportCooldown -= timePassed;
    //move asteroids
    vector<Asteroid> *asteroids = previous->asteroids;
    for(unsigned int k = 0; k < asteroids->size(); k++)
    {
        updateLocation(asteroids->at(k).position, asteroids->at(k).velocity, timePassed);
    }

    //move and age bullets
    vector<Bullet> *bullets = previous->bullets;
    for(unsigned int k = 0; k < bullets->size(); k++)
    {
        updateLocation(bullets->at(k).position, bullets->at(k).velocity, timePassed);
        bullets->at(k).age += timePassed;
        if(bullets->at(k).age > bulletAge)
        {
            bullets->erase(bullets->begin() + k);
        }
    }

    //move ship
    updateLocation(previous->shipLoc, previous->shipVelocity, timePassed);

    //detect collisions

    //update based on controls
    if(previous->turningLeft && !previous->turningRight)
    {
        previous->shipAngle -= turnRate * timePassed;
        if(previous->shipAngle < 0)
        {
            previous->shipAngle += 360;
        }
    }
    else if(previous->turningRight && !previous->turningLeft)
    {
        previous->shipAngle += turnRate * timePassed;
        if(previous->shipAngle > 360)
        {
            previous->shipAngle -= 360;
        }
    }
    if(previous->thrusting)
    {
        updateVelocity(previous->shipVelocity, previous->shipAngle, thrust * timePassed);
    }
    else
    {
        if(previous->shipVelocity->x >= 0)
        {
            previous->shipVelocity->x -= friction * timePassed;
        }
        else
        {
            previous->shipVelocity->x += friction * timePassed;
        }
        
        if(previous->shipVelocity->y >= 0)
        {
            previous->shipVelocity->y -= friction * timePassed;
        }
        else
        {
            previous->shipVelocity->y += friction * timePassed;
        }
    }
    if(previous->firing)
    {
        if(previous->lastFiredCooldown < 0 && previous->bullets->size() < maxBullets)
        {
            Bullet *bullet = new Bullet();
            bullet->position = new vec2(previous->shipLoc->x + cos((previous->shipAngle - 90) * PI / 180) * 15,
                                        previous->shipLoc->y + sin((previous->shipAngle - 90) * PI / 180) * 15);
            double vel = sqrt(pow(previous->shipVelocity->x, 2) + pow(previous->shipVelocity->y, 2)) + bulletSpeed;
            bullet->velocity = new vec2(cos((previous->shipAngle - 90) * PI / 180) * vel, sin((previous->shipAngle - 90) * PI / 180) * vel);
            previous->lastFiredCooldown = fireRate;
            previous->bullets->push_back(*bullet);
        }
    }
    if(previous->teleporting)
    {
        if(previous->teleportCooldown < 0)
        {
            previous->shipLoc->x = rand() % width;
            previous->shipLoc->y = rand() % height;
            previous->shipVelocity->x = 0;
            previous->shipVelocity->y = 0;
            previous->teleportCooldown = teleportCooldown;
        }
    }
}

void GameEngine::updateLocation(vec2 *original, vec2 *velocity, double time)
{
    original->x = original->x + (velocity->x * time);
    if(original->x > width)
    {
        original->x = original->x - width;
    }
    else if(original->x < 0)
    {
        original->x = original->x + width;
    }

    original->y = original->y + (velocity->y * time);
    if(original->y > height)
    {
        original->y = original->y - height;
    }
    else if(original->y < 0)
    {
        original->y = original->y + height;
    }
}

void GameEngine::updateVelocity(vec2 *velocity, double angle, double add)
{
    velocity->x += cos((angle - 90) * PI / 180) * add;
    velocity->y += sin((angle - 90) * PI / 180) * add;
}

vector<Asteroid> * GameEngine::createAsteroids(int number, vec2 *shipLoc)
{
    vector<Asteroid> *asteroids = new vector<Asteroid>();
    for(int k = 0; k < number; k++)
    {
        vec2 *tempPoint = new vec2(rand() % width, rand() % height);
        while((tempPoint->x - shipLoc->x) * (tempPoint->x - shipLoc->x) + (tempPoint->y - shipLoc->y) * (tempPoint->y - shipLoc->y) < 50 * 50)
        {
            delete tempPoint;
            tempPoint = new vec2(rand() % width, rand() % height);
        }

        asteroids->push_back(*(createAsteroid(tempPoint, baseAsteroidRadius, asteroidSpeed)));
    }

    return asteroids;
}

Asteroid * GameEngine::createAsteroid(vec2 *center, double radius, double velocity)
{
    Asteroid *asteroid = new Asteroid();
    asteroid->position = center;
    asteroid->radius = radius;
    double angle = abs(rand() % 360) * PI / 180;
    asteroid->velocity = new vec2(velocity * cos(angle), velocity * sin(angle));
    asteroid->points = new vector<vec2>();
    
    int sides = 7 + (abs(rand() % 5));
    double change = 360. / sides;
    angle = rand() % (int)(change / 2);
    angle = angle * PI / 180;
    for(int k = 0; k < sides; k++)
    {
        double tempRadius = radius;
        if(abs(rand()) % sides < 2)
        {
            tempRadius = ((25 + abs(rand()) % 75) / 100.) * radius;
        }
        vec2 *tempPoint = new vec2(tempRadius * cos(angle), tempRadius * sin(angle));
        asteroid->points->push_back(*tempPoint);
        angle = (k + 1) * change;
        angle += rand() % (int)(change / 2);
        angle = angle * PI / 180;
    }

    return asteroid;
}
