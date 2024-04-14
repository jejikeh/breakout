#pragma once

#include "../base.h"
#include "entity.h"

#define BLOCKS_HEALTH_MAX 1.0f

enum class BlockState
{
    Dead,
    Normal,
    WithBonus
};

struct Block : Entity
{
    BlockState state = BlockState::Dead;

    Block(Arkanoid* a, int x, int y) : Entity(a)
    {
        transform.pos.x = x;
        transform.pos.y = y;
    }

    void reset(const ArkanoidSettings& settings) override
    {
        transform.size = settings.calculate_brick_size();
    }
};