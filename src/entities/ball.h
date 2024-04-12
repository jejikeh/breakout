#pragma once

#include "entity.h"
#include "../utils.h"

// struct BallModifier;

#define BALL_DAMAGE_MULTIPLIER 0.33f

struct Ball : public Entity
{
    float radius = 0.0f;
    float initial_speed = 0.0f;

    Vect velocity = Vect(0.0f);

    Ball(int x, int y)
    {
        transform.pos.x = x;
        transform.pos.y = y;
    }

    void reset(const ArkanoidSettings& settings) override
    {
        if (transform.pos == Vect(0.0f))
        {
            transform.pos = Entity::world_space.world_size / 2.0f + Vect(0, 100.0f);
        }

        radius = settings.ball_radius;
        initial_speed = settings.ball_speed;
        velocity = Vect(-initial_speed);
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
        }

        // @Note(jejkeh): This can be removed to speedup ball after some time.
        if (velocity.x > initial_speed)
        {
            velocity.x = initial_speed;
        }
    }

    bool check_ball_collision_with_transform(const Transform& other) const
    {
        const auto closestX = std::clamp(transform.pos.x, other.pos.x, other.pos.x + other.size.x);
        const auto closestY = std::clamp(transform.pos.y, other.pos.y, other.pos.y + other.size.y);

        Vect distance = Vect(transform.pos.x - closestX, transform.pos.y - closestY);

        return distance.LengthSquared() < (radius * radius);
    }

    // @Note(jejikeh): I want keep things realy simple, so
    // i decide to not add some complex logic to Entities.
    // Because this leads to more complex entity managment.

    // void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) override;

    // void draw(ImGuiIO& io, ImDrawList& draw_list) override;

    // private:
    // @Note(jejikeh): Yes, i know, vectors... This can be rewritten using arena or something like that...
    // std::vector<BallModifier> modifiers;
};

// enum class BallModifierState : uint8_t
// {
//     None,
//     SinMovement,
//     CosMovement,
//     RandomOffsets
// };

// struct BallModifier
// {
//     Ball* ball;
//     bool enabled = true;
//     BallModifierState state = BallModifierState::None;

//     void update(float elapsed);
// };
