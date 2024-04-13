#pragma once

#include "entity.h"
#include "../utils.h"

#define MIN_BALL_RADIUS 1.5f
#define MAX_BALL_RADIUS 20.0f

#define BALL_DAMAGE_MULTIPLIER 0.33f

struct Ball : public Entity
{
    CircleCollider* circle_collider;

    float initial_speed = 0.0f;
    float speed_modifier = 1.0f;
    Vect velocity = Vect(0.0f);

    Ball(int x, int y)
    {
        transform.pos.x = x;
        transform.pos.y = y;

        circle_collider = new CircleCollider(&transform);
    }

    void reset(const ArkanoidSettings& settings) override
    {
        if (transform.pos == Vect(0.0f))
        {
            transform.pos = Entity::world_space.world_size / 2.0f + Vect(0, 100.0f);
        }

        transform.size.x = settings.ball_radius;

        initial_speed = settings.ball_speed;
        velocity = Vect(-initial_speed * speed_modifier);

        circle_collider->set_radius(settings.ball_radius);
    }

    void rebound_relative_to_normal(Vect normal)
    {
        velocity = velocity - 2.0f * (DOT(normal, velocity)) * normal;
        keep_velocity_above_initial_speed();
    }

    void keep_velocity_above_initial_speed()
    {
        // @Note(jejikeh): I specificly handle here abs y, because if we get some bad angle, ball will be wery slowly
        // aproach the bottom of the screen, and thats not fun.
        if (abs(velocity.y) < initial_speed)
        {
            velocity.y = initial_speed * (velocity.y > 0.0f ? 1.0f : -1.0f);
            velocity.y *= speed_modifier;
        }

        // @Note(jejkeh): This can be removed to speedup ball after some time.
        if (velocity.x > initial_speed)
        {
            velocity.x = initial_speed * speed_modifier;
        }
    }

    void set_radius(float radius)
    {
        radius = std::clamp(radius, MIN_BALL_RADIUS, MAX_BALL_RADIUS);
        circle_collider->set_radius(radius);
    }

    float radius() const
    {
        return circle_collider->radius;
    }

    // @Note(jejikeh): I want keep things realy simple, so
    // i decide to not add some complex logic to Entities.
    // Because this leads to more complex entity managment.

    // void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) override;

    // void draw(ImGuiIO& io, ImDrawList& draw_list) override;
};