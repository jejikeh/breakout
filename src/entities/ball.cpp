#include "ball.h"

Ball::Ball()
{
    modifiers.push_back(BallModifier{this, true, BallModifierState::SinMovement});
    modifiers.push_back(BallModifier{this, true, BallModifierState::CosMovement});
}

void Ball::update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    // update ball position according
    // its velocity and elapsed time
    transform.pos += velocity * elapsed;

    printf("ball pos %f %f\n", transform.pos.x, transform.pos.y);

    if (transform.pos.x < radius)
    {
        transform.pos.x += (radius - transform.pos.x) * 2.0f;
        velocity.x *= -1.0f;

        printf("1 ball velocity %f\n", velocity.x);
    }
    else if (transform.pos.x > (Entity::world_space.world_size.x - radius))
    {
        transform.pos -= (transform.pos.x - (Entity::world_space.world_size.x - radius)) * 2.0f;
        velocity.x *= -1.0f;

        printf("2 ball velocity %f\n", velocity.x);
    }

    if (transform.pos.y < radius)
    {
        transform.pos.y += (radius - transform.pos.y) * 2.0f;
        velocity.y *= -1.0f;

        printf("3 ball velocity %f\n", velocity.y);
    }
    else if (transform.pos.y > (Entity::world_space.world_size.y - radius))
    {
        transform.pos.y -= (transform.pos.y - (Entity::world_space.world_size.y - radius)) * 2.0f;
        velocity.y *= -1.0f;

        printf("4 ball velocity %f\n", velocity.y);
    }

    // update modifiers
    for (auto& modifier : modifiers)
    {
        modifier.update(elapsed);
    }
}

void Ball::draw(ImGuiIO& io, ImDrawList& draw_list)
{
    Vect screen_pos = transform.pos * Entity::world_space.world_to_screen;

    float screen_radius = radius * Entity::world_space.world_to_screen.x;

    draw_list.AddCircleFilled(screen_pos, screen_radius, ImColor(100, 255, 100));
}

void BallModifier::update(float elapsed)
{
    if (!enabled || state == BallModifierState::None)
    {
        return;
    }

    if (state == BallModifierState::SinMovement)
    {
        ball->velocity.y += ball->initial_speed * sinf(elapsed * float(rand() % 10));
        ball->velocity.x += ball->initial_speed * sinf(elapsed * float(rand() % 10));
    }

    if (state == BallModifierState::CosMovement)
    {
        ball->velocity.y -= ball->initial_speed * cosf(elapsed * float(rand() % 10));
        ball->velocity.x -= ball->initial_speed * cosf(elapsed * float(rand() % 10));
    }

    if (state == BallModifierState::RandomOffsets)
    {
        ball->velocity.y += (rand() % 10 - 5) * elapsed;
        ball->velocity.x += (rand() % 10 - 5) * elapsed;
    }
}