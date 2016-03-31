#ifndef BULLET_H
#define BULLET_H
#include "object.h"

class Bullet: public Object
{
    public:
        Bullet(vec2 center, vec2 vel, int index, double age);
};
#endif
