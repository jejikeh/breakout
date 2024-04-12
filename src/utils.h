#pragma once
#include "base.h"

float Dot(const Vect& a, const Vect& b)
{
    return (a.x * b.x + a.y * b.y);
}