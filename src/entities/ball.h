#pragma once

#include "entity.h"

struct BallModifier;

struct Ball : public Entity
{
    float radius = 0.0f;
    float initial_speed = 0.0f;

    Vect velocity = Vect(0.0f);

    Ball();

    void reset(const ArkanoidSettings& settings) override
    {
        if (transform.pos == Vect(0.0f))
        {
            transform.pos = Entity::world_space.world_size / 2.0f;
        }

        radius = settings.ball_radius;
        initial_speed = settings.ball_speed;
        velocity = Vect(initial_speed);

        printf("velocity = (%f, %f)\n", velocity.x, velocity.y);
    }

    void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) override;
    void draw(ImGuiIO& io, ImDrawList& draw_list) override;

private:
    // @Note(jejikeh): Yes, i know, vectors... This can be rewritten using arena allocators.
    std::vector<BallModifier> modifiers;
};

enum class BallModifierState : uint8_t
{
    None,
    SinMovement,
    CosMovement,
    RandomOffsets
};

struct BallModifier
{
    Ball* ball;
    bool enabled = true;
    BallModifierState state = BallModifierState::None;

    void update(float elapsed);
};
