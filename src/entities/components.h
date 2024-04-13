#pragma once

#include "../base.h"
#include "../arkanoid.h"

struct Transform
{
    Vect pos;
    Vect size;

    Transform() = default;

    void set_position(const Vect& pos)
    {
        this->pos = pos;
    }

    void set_size(const Vect& s)
    {

        this->size = s;
    }
};

struct CircleCollider
{
    Transform* transform;

    float radius;

    CircleCollider(Transform* t)
    {
        transform = t;
    }

    bool check_collision_with_transform(const Transform& other) const
    {
        const auto closestX = std::clamp(transform->pos.x, other.pos.x, other.pos.x + other.size.x);
        const auto closestY = std::clamp(transform->pos.y, other.pos.y, other.pos.y + other.size.y);

        Vect distance = Vect(transform->pos.x - closestX, transform->pos.y - closestY);

        return distance.LengthSquared() < (radius * radius);
    }

    void set_radius(float radius)
    {
        this->radius = radius;
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