#pragma once

#include "../base.h"
#include "../arkanoid.h"

struct Transform
{
    Vect pos;
    Vect size;

    Transform() = default;

    void set(const Vect& pos)
    {
        this->pos = pos;
    }
};

struct WorldSpace
{
    Vect world_size = Vect(0.0f);
    Vect world_to_screen = Vect(0.0f);

    WorldSpace() = default;

    void set(const Vect& world_size, const Vect& world_to_screen)
    {
        this->world_size = world_size;
        this->world_to_screen = world_to_screen;
    }
};