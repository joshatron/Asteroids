#include <stdlib.h>
#include "gameengine.h"
#include <iostream>
#include <cmath>

#define PI 3.14159265

using std::cout;
using std::endl;

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
    maxBullets = 4;
    baseAsteroids = 4;
    width = 1600;
    height = 900;
    srand(time(NULL));
}

void GameEngine::createInitialState(GameState& state)
{
    //initialize ship
    Ship mainShip;
    mainShip.position.x = width / 2;
    mainShip.position.y = height / 2;
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
    mainShip.teleporting = false;
    mainShip.firing = false;
    mainShip.fired = false;
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
    mainShip.bulletFirePoint.x = -15;

    state.ships.push_back(main_ship);

    //initialize asteroids
    state.numAsteroids = baseAsteroids;
    createAsteroids(state, state.numAsteroids);

    //wait to start game for 3 seconds
    state.pauseTime = 3.;
}

void GameEngine::getNextState(GameState& state, double timePassed)
{
    updateObjects(state, timePassed);
    detectCollisions(state);
}

void GameEngine::updateObjects(GameState& state, double timePassed)
{
    previous->lastFiredCooldown -= timePassed;
    previous->teleportCooldown -= timePassed;
    previous->pauseTime -= timePassed;
    //move asteroids

    if(previous->pauseTime > 0)
    {
        return;
    }

    if(previous->shipLoc->x < -50)
    {
        bool move = true;
        for(unsigned int k = 0; k < previous->asteroids->size(); k++)
        {
            if(sqrt(pow(width / 2 - previous->asteroids->at(k).position->x, 2) + pow(height / 2 - previous->asteroids->at(k).position->y, 2)) < 100)
            {
                move = false;
                previous->teleportCooldown = 10;
                previous->lastFiredCooldown = 10;
                break;
            }
        }

        if(move)
        {
            previous->shipLoc = new vec2(width / 2, height / 2);
            previous->teleportCooldown = -1.;
            previous->lastFiredCooldown = -1.;
        }
    }
    else
    {
        updateLocation(previous->shipLoc, previous->shipVelocity, timePassed);
    }

    if(previous->asteroids->size() == 0)
    {
        delete previous->asteroids;
        previous->numAsteroids += 2;
        previous->asteroids = createAsteroids(previous->numAsteroids, previous->shipLoc);
    }
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
            bullet->velocity = new vec2(cos((previous->shipAngle - 90) * PI / 180) * bulletSpeed + previous->shipVelocity->x,
                                        sin((previous->shipAngle - 90) * PI / 180) * bulletSpeed + previous->shipVelocity->y);
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

void GameEngine::detectCollisions(GameState& state)
{
    //detect collisions
    for(unsigned int k = 0; k < bullets->size(); k++)
    {
        for(unsigned int a = 0; a < asteroids->size(); a++)
        {
            if(sqrt(pow(bullets->at(k).position->x - asteroids->at(a).position->x, 2) +
                    pow(bullets->at(k).position->y - asteroids->at(a).position->y, 2)) < asteroids->at(a).radius)
            {
                bullets->erase(bullets->begin() + k);
                
                if(asteroids->at(a).radius + .1 > baseAsteroidRadius)
                {
                    asteroids->push_back(*(createAsteroid(new vec2(asteroids->at(a).position->x, asteroids->at(a).position->y),
                                         baseAsteroidRadius / 2, asteroidSpeed * 2)));
                    asteroids->push_back(*(createAsteroid(new vec2(asteroids->at(a).position->x, asteroids->at(a).position->y),
                                         baseAsteroidRadius / 2, asteroidSpeed * 2)));
                    asteroids->erase(asteroids->begin() + a);
                }
                else if(asteroids->at(a).radius + .1 > baseAsteroidRadius / 2)
                {
                    asteroids->push_back(*(createAsteroid(new vec2(asteroids->at(a).position->x, asteroids->at(a).position->y),
                                         baseAsteroidRadius / 3, asteroidSpeed * 3)));
                    asteroids->push_back(*(createAsteroid(new vec2(asteroids->at(a).position->x, asteroids->at(a).position->y),
                                         baseAsteroidRadius / 3, asteroidSpeed * 3)));
                    asteroids->erase(asteroids->begin() + a);
                }
                else
                {
                    asteroids->erase(asteroids->begin() + a);
                }

                break;
            }
        }
    }

    vec2 front(previous->shipLoc->x + cos((previous->shipAngle - 90) * PI / 180) * 15,
               previous->shipLoc->y + sin((previous->shipAngle - 90) * PI / 180) * 15);
    vec2 back1(previous->shipLoc->x + cos((previous->shipAngle + 61.928) * PI / 180) * 15,
               previous->shipLoc->y + sin((previous->shipAngle + 61.928) * PI / 180) * 15);
    vec2 back2(previous->shipLoc->x + cos((previous->shipAngle + 28.072 + 90) * PI / 180) * 15,
               previous->shipLoc->y + sin((previous->shipAngle + 28.072 + 90) * PI / 180) * 15);
    for(unsigned int a = 0; a < asteroids->size(); a++)
    {
        if(sqrt(pow(front.x - asteroids->at(a).position->x, 2) + pow(front.y - asteroids->at(a).position->y, 2)) < asteroids->at(a).radius ||
           sqrt(pow(back1.x - asteroids->at(a).position->x, 2) + pow(back1.y - asteroids->at(a).position->y, 2)) < asteroids->at(a).radius || 
           sqrt(pow(back2.x - asteroids->at(a).position->x, 2) + pow(back2.y - asteroids->at(a).position->y, 2)) < asteroids->at(a).radius ||
           sqrt(pow(previous->shipLoc->x - asteroids->at(a).position->x, 2) +
           pow(previous->shipLoc->y - asteroids->at(a).position->y, 2)) < asteroids->at(a).radius)
        {
            previous->shipLoc->x = -100;
            previous->shipLoc->y = -100;
            previous->shipVelocity->x = 0;
            previous->shipVelocity->y = 0;
            previous->shipAngle = 0;
            previous->livesLeft--;
            cout << previous->livesLeft << endl;

            if(asteroids->at(a).radius + .1 > baseAsteroidRadius)
            {
                asteroids->push_back(*(createAsteroid(new vec2(asteroids->at(a).position->x, asteroids->at(a).position->y),
                                     baseAsteroidRadius / 2, asteroidSpeed * 2)));
                asteroids->push_back(*(createAsteroid(new vec2(asteroids->at(a).position->x, asteroids->at(a).position->y),
                                     baseAsteroidRadius / 2, asteroidSpeed * 2)));
                asteroids->erase(asteroids->begin() + a);
            }
            else if(asteroids->at(a).radius + .1 > baseAsteroidRadius / 2)
            {
                asteroids->push_back(*(createAsteroid(new vec2(asteroids->at(a).position->x, asteroids->at(a).position->y),
                                     baseAsteroidRadius / 3, asteroidSpeed * 3)));
                asteroids->push_back(*(createAsteroid(new vec2(asteroids->at(a).position->x, asteroids->at(a).position->y),
                                     baseAsteroidRadius / 3, asteroidSpeed * 3)));
                asteroids->erase(asteroids->begin() + a);
            }
            else
            {
                asteroids->erase(asteroids->begin() + a);
            }

            break;
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

void GameEngine::createAsteroids(GameState& state, int number)
{
    for(int k = 0; k < number; k++)
    {
        vec2 tempPoint = vec2(rand() % width, rand() % height);
        bool done = false;
        while(!done)
        {
            done = true;
            for(int a = 0; a < state.ships.size(); a++)
            {
                if(sqrt(pow((tempPoint.x - state.ships[a].position.x), 2) + pow((tempPoint->y - state.ships[a].position.y), 2)) < 100)
                {
                    done = false;
                    tempPoint->x = rand() % width;
                    tempPoint->y = rand() % height;
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
    asteroid.velocity.x = velocity * cos(angle)
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
