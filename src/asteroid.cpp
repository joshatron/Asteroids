#include "asteroid.h"

GameEngine::Asteroid(vec2 center, double radius, double velocity):
Object(center, vec2(0,0), 0)
{
    //this method uses degrees instead of radians because it is easier to work with integers moving around a circle
    ConvexShape currentShape;
    currentShape.points.push_back(vec2(0,0));

    radius = radius;
    double ang = abs(rand() % 360) * PI / 180;
    velocity.x = velocity * cos(ang);
    velocity.y = velocity * sin(ang);

    int last = -999;
    int sides = 7 + (abs(rand() % 5));
    double change = 360. / sides;
    ang = rand() % (int)(change / 2);
    ang = angle * PI / 180;
    vec2 first;
    for(int k = 0; k < sides; k++)
    {
        double tempRadius = radius;
        if(abs(rand()) % sides < 2 && last + 2 != k && last + 3 != k)
        {
            tempRadius = ((50 + abs(rand()) % 50) / 100.) * radius;
            last = k;
        }

        vec2 tempPoint = vec2(tempRadius * cos(ang), tempRadius * sin(ang));

        if(k == 0)
        {
            first.x = tempPoint.x;
            first.y = tempPoint.y;
            points.push_back(tempPoint);
        }
        else
        {
            points.push_back(tempPoint);
            points.push_back(tempPoint);
        }

        currentShape.points.push_back(tempPoint);

        if(last == k)
        {
            collisionShapes.push_back(currentShape);
            currentShape.points.clear();
            currentShape.points.push_back(vec2(0,0));
            currentShape.points.push_back(tempPoint);
        }

        ang = (k + 1) * change;
        ang += rand() % (int)(change / 2);
        ang = angle * PI / 180;

    }

    points.push_back(first);

    if(currentShape.points.size() == points.size() + 1)
    {
        currentShape.points.erase(currentShape.points.begin());
    }
    else
    {
        currentShape.points.push_back(points.at(0));
    }

    collisionShapes.push_back(currentShape);
}

vector<shared_ptr<Object>> Asteroid::destroy(const GameState& state, int region)
{
    vector<shared_ptr<Object>> toReturn;

    if(radius + .1 > state.baseAsteroidRadius)
    {
        toReturn.push_back(createAsteroid(state, position, state.baseAsteroidRadius / 2, state.asteroidSpeed * 2));
        toReturn.push_back(createAsteroid(state, position, state.baseAsteroidRadius / 2, state.asteroidSpeed * 2));
        //addScore(state, 25);
        //state.stats->largeAsteroidsDestroyed++;
    }
    //if it was a middle one 
    else if(asteroid->radius + .1 > state.baseAsteroidRadius / 2)
    {
        toReturn.push_back(createAsteroid(state, position, state.baseAsteroidRadius / 3, state.asteroidSpeed * 3));
        toReturn.push_back(createAsteroid(state, position, state.baseAsteroidRadius / 3, state.asteroidSpeed * 3));
        //addScore(state, 50);
        //state.stats->mediumAsteroidsDestroyed++;
    }
    //if it was the smallest one
    else
    {
        //addScore(state, 100);
        //state.stats->smallAsteroidsDestroyed++;
    }

    return toReturn;
}

