#include "arkanoid_impl.h"

#include <GLFW/glfw3.h>
#include "utils.h"

WorldSpace Entity::world_space = {};

#ifdef USE_ARKANOID_IMPL
Arkanoid* create_arkanoid()
{
    return new ArkanoidImpl();
}
#endif

ArkanoidImpl::ArkanoidImpl()
{
    balls.push_back(new Ball(10, 245));
    // balls.push_back(new Ball(49, 55));

    // blocks.push_back(new Block(100, 100));
    // balls.push_back(Ball());
}

void ArkanoidImpl::reset(const ArkanoidSettings& settings)
{
    // TODO:
    // Implement your game world, bricks and
    // carriage initialization
    // ...

    // TODO:
    // remove demo code
    Entity::world_space.world_size.x = settings.world_size[0];
    Entity::world_space.world_size.y = settings.world_size[1];

    for (auto& ball : balls)
    {
        ball->reset(settings);
    }

    init_blocks(settings);

    for (auto& block : blocks)
    {
        block->reset(settings);
    }
}

void ArkanoidImpl::update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    Entity::world_space.world_to_screen =
        Vect(io.DisplaySize.x / Entity::world_space.world_size.x, io.DisplaySize.y / Entity::world_space.world_size.y);

    // TODO:
    // Implement you Arkanoid user input handling
    // and game logic.
    // ...

    // TODO:
    // remove demo code
    // demo_update(io, debug_data, elapsed);

    // for (auto& ball : balls)
    // {
    //     ball.update(io, debug_data, elapsed);
    // }
    update_balls(io, debug_data, elapsed);
}

void ArkanoidImpl::draw(ImGuiIO& io, ImDrawList& draw_list)
{
    // TODO:
    // Implement you Arkanoid drawing
    // ...

    // TODO:
    // remove demo code
    // demo_draw(io, draw_list);

    // for (auto& ball : balls)
    // {
    //     ball->draw(io, draw_list);
    // }

    draw_balls(io, draw_list);

    draw_blocks(io, draw_list);
}

void ArkanoidImpl::draw_balls(ImGuiIO& io, ImDrawList& draw_list)
{
    for (auto& ball : balls)
    {
        Vect screen_pos = ball->transform.pos * Entity::world_space.world_to_screen;

        float screen_radius = ball->radius * Entity::world_space.world_to_screen.x;

        draw_list.AddCircle(screen_pos, screen_radius, ImColor(100, 255, 100));
    }
}

void ArkanoidImpl::draw_blocks(ImGuiIO& io, ImDrawList& draw_list) const
{
    for (auto& block : blocks)
    {
        if (block->state == BlockState::Dead)
        {
            continue;
        }

        const auto screen_pos = block->transform.pos * Entity::world_space.world_to_screen;

        const auto size = block->transform.size * Entity::world_space.world_to_screen;

        const auto color = block->state == BlockState::WithBonus ? ImColor(0, 255, 0) : ImColor(255, 0, 0);

        draw_list.AddRect(screen_pos, screen_pos + size, color);
    }
}

void ArkanoidImpl::update_balls(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    for (auto& ball : balls)
    {
        // update ball position according
        // its velocity and elapsed time
        ball->transform.pos += ball->velocity * elapsed;

        if (ball->transform.pos.x < ball->radius)
        {
            ball->transform.pos.x += (ball->radius - ball->transform.pos.x) * 2.0f;
            ball->velocity.x *= -1.0f;

            add_debug_hit(debug_data, ball->transform.pos, Vect(1.0f, 0.0f));
        }
        else if (ball->transform.pos.x > (Entity::world_space.world_size.x - ball->radius))
        {
            ball->transform.pos -= (ball->transform.pos.x - (Entity::world_space.world_size.x - ball->radius)) * 2.0f;
            ball->velocity.x *= -1.0f;

            add_debug_hit(debug_data, ball->transform.pos, Vect(-1.0f, 0.0f));
        }

        if (ball->transform.pos.y < ball->radius)
        {
            ball->transform.pos.y += (ball->radius - ball->transform.pos.y) * 2.0f;
            ball->velocity.y *= -1.0f;

            add_debug_hit(debug_data, ball->transform.pos, Vect(0.0f, 1.0f));
        }
        else if (ball->transform.pos.y > (Entity::world_space.world_size.y - ball->radius))
        {
            ball->transform.pos.y -= (ball->transform.pos.y - (Entity::world_space.world_size.y - ball->radius)) * 2.0f;
            ball->velocity.y *= -1.0f;

            add_debug_hit(debug_data, ball->transform.pos, Vect(0.0f, -1.0f));
        }

        for (auto& block : blocks)
        {
            if (block->state == BlockState::Dead)
            {
                continue;
            }

            if (ball->check_ball_collision_with_transform(block->transform))
            {
                Vect normal = ball->transform.pos - block->transform.pos;
                normal.Normalize();

                ball->rebound_relative_to_normal(normal);

                block->state = BlockState::Dead;

                add_debug_hit(debug_data, ball->transform.pos, normal);
            }
        }
    }
}

float ArkanoidImpl::calculate_offset_to_center(const ArkanoidSettings& settings, Vect block_size)
{
    return Entity::world_space.world_size.x / 2.0f -
           (settings.bricks_columns_count * (((block_size.x) + settings.bricks_columns_padding) / 2.0f));
}

void ArkanoidImpl::init_blocks(const ArkanoidSettings& settings)
{
    // @Note(jejikeh): Initialize bricks only once
    if (blocks.empty())
    {
        for (int i = 0; i < settings.bricks_columns_max * settings.bricks_rows_max; i++)
        {
            blocks.push_back(new Block(0, 0));
        }
    }
    else
    {
        for (auto& block : blocks)
        {
            block->state = BlockState::Dead;
        }
    }

    // @Note(jejikeh): I decide to put bricks in the center, so they will cover the whole screen in any settings.
    auto block_size = settings.calculate_brick_size();

    auto center_offset_x = calculate_offset_to_center(settings, block_size);

    for (int y = 0; y < settings.bricks_rows_count; y++)
    {
        const auto pos_y = y * block_size.y + settings.bricks_rows_padding * (y + 1);

        for (int x = 0; x < settings.bricks_columns_count; x++)
        {
            auto block = blocks.at(y * settings.bricks_columns_count + x);

            block->transform.pos.x = x * block_size.x + settings.bricks_columns_padding * (x + 1) + center_offset_x;
            block->transform.pos.y = pos_y;
            block->state = static_cast<BlockState>((rand() % 2) + 1);
        }
    }
}

void ArkanoidImpl::add_debug_hit(ArkanoidDebugData& debug_data, const Vect& world_pos, const Vect& normal)
{
    ArkanoidDebugData::Hit hit;
    hit.screen_pos = world_pos * Entity::world_space.world_to_screen;
    hit.normal = normal;
    debug_data.hits.push_back(std::move(hit));
}