#include <stdlib.h>
#include "gameengine.h"
#include <iostream>

#define PI 3.14159265

GameEngine::GameEngine()
{
    friction = 1;
    thrust = 10;
    bulletSpeed = 200;
    asteroidSpeed = 50;
    baseAsteroidRadius = 30;
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
    state->timeLastFired = 0.;
    state->turningLeft = false;
    state->turningRight = false;
    state->thrusting = false;
    state->firing = false;
    state->teleporting = false;

    return state;
}

void GameEngine::getNextState(GameState *previous, double timePassed)
{
    vector<Asteroid> *asteroids = previous->asteroids;
    for(unsigned int k = 0; k < asteroids->size(); k++)
    {
        updateLocation(asteroids->at(k).position, asteroids->at(k).velocity, timePassed);
    }

    vector<Bullet> *bullets = previous->bullets;
    for(unsigned int k = 0; k < bullets->size(); k++)
    {
        updateLocation(bullets->at(k).position, bullets->at(k).velocity, timePassed);
    }

    updateLocation(previous->shipLoc, previous->shipVelocity, timePassed);
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
