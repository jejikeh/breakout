#pragma once

#include "entity.h"

struct Pad : Entity
{
    float max_velocity = 0.0f;
    float target_velocity = 100.0f;
    float current_velocity = 0.0f;

    float acceleration = 1.0f;

    Pad(int x, int y)
    {
        transform.pos.x = x;
        transform.pos.y = y;
    }

    void reset(const ArkanoidSettings& settings) override
    {
        max_velocity = settings.carriage_speed;
        acceleration = settings.carriage_acceleration;

        transform.size.x = settings.carriage_width;
        transform.size.y = settings.calculate_brick_size().y;

        transform.pos.x = settings.world_size[0] / 2.0f - transform.size.x / 2.0f;
        transform.pos.y = settings.world_size[1] - transform.size.y - settings.bricks_rows_padding * 2.0f;
    }
};