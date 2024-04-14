#pragma once

#include "../arkanoid.h"
#include "components.h"

struct Entity
{
    Arkanoid* arkanoid;
    Transform transform;

    Entity(Arkanoid* a)
    {
        arkanoid = a;
    }

    virtual void reset(const ArkanoidSettings& settings) = 0;
};