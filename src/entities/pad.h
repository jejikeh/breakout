#pragma once

#include "entity.h"

struct Pad : Entity
{
    float max_velocity;
    float current_velocity;
    float target_velocity = 100.0f;

    float speed_modifier = 1.0f;

    float acceleration = 1.0f;

    Pad(Arkanoid* a, int x, int y) : Entity(a)
    {
        transform.pos.x = x;
        transform.pos.y = y;
    }

    void reset(const ArkanoidSettings& settings) override
    {
        max_velocity = settings.carriage_speed * speed_modifier;
        acceleration = settings.carriage_acceleration;

        transform.size.x = settings.carriage_width;
        transform.size.y = settings.calculate_brick_size().y;

        speed_modifier = 1.0f;
    }

    void set_width(float width)
    {
        transform.size.x = std::clamp(width, ArkanoidSettings::carriage_width_min, ArkanoidSettings::carriage_width_max);
    }
};