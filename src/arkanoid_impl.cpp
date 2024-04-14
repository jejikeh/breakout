#include "arkanoid_impl.h"

#include <GLFW/glfw3.h>
#include "utils.h"
#include <imgui_internal.h>

#ifdef USE_ARKANOID_IMPL
Arkanoid* create_arkanoid()
{
    return new ArkanoidImpl();
}
#endif

ArkanoidImpl::ArkanoidImpl()
{
    balls.push_back(std::make_unique<Ball>(this, 0, 0));

    pad = std::make_unique<Pad>(this, 0, 0);

    current_settings = {};

    bonuse_message = std::make_unique<BonusUIMessage>(this);
}

void ArkanoidImpl::reset(const ArkanoidSettings& settings)
{
    current_settings = settings;

    world_space.world_size = settings.world_size;

    game_state.reset();

    // @Note(jejikeh): delete all balls except the first one
    balls.erase(balls.begin() + 1, balls.end());
    balls.back()->reset(settings);

    init_blocks(settings);
    for (auto& block : blocks)
    {
        block->reset(settings);
    }

    pad->reset(settings);
    pad->transform.pos.x = settings.world_size.x / 2.0f - pad->transform.size.x / 2.0f;
    pad->transform.pos.y = settings.world_size.y - pad->transform.size.y - settings.bricks_rows_padding * 2.0f;

    bonuses.clear();

    bonuse_message->reset(settings);
}

void ArkanoidImpl::update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    world_space.world_to_screen = Vect(io.DisplaySize.x / world_space.world_size.x, io.DisplaySize.y / world_space.world_size.y);

    switch (game_state.state)
    {
    case ArkanoidState::Waiting:
    {
        if (io.KeysDown[GLFW_KEY_SPACE])
        {
            game_state.state = ArkanoidState::Playing;
        }

        break;
    }
    case ArkanoidState::Playing:
    {
        update_balls(io, debug_data, elapsed);
        update_pad(io, debug_data, elapsed);
        update_bonuses(io, debug_data, elapsed);
        update_bonuses_message(io, debug_data, elapsed);

        if (io.KeysDown[GLFW_KEY_ESCAPE])
        {
            game_state.state = ArkanoidState::Waiting;
        }

        if (game_state.lives <= 0)
        {
            game_state.state = ArkanoidState::Lost;
        }

        if (game_state.blocks_destroyed >= current_settings.bricks_columns_count * current_settings.bricks_rows_count)
        {
            game_state.state = ArkanoidState::Won;
        }

        break;
    }
    case ArkanoidState::Won:
    {
        if (io.KeysDown[GLFW_KEY_SPACE])
        {
            reset(current_settings);
            game_state.state = ArkanoidState::Playing;
        }

        break;
    }
    case ArkanoidState::Lost:
    {
        if (io.KeysDown[GLFW_KEY_SPACE])
        {
            reset(current_settings);
            game_state.state = ArkanoidState::Playing;
        }
    }
    }
}

void ArkanoidImpl::draw(ImGuiIO& io, ImDrawList& draw_list)
{
    switch (game_state.state)
    {
    case ArkanoidState::Waiting:
    {
        draw_balls(io, draw_list);
        draw_pad(io, draw_list);
        draw_blocks(io, draw_list);

        draw_text_on_white_background(
            draw_list, "Press SPACE to start game", current_settings.world_size[0] / 2, current_settings.world_size[1] / 2);

        break;
    }
    case ArkanoidState::Playing:
    {
        draw_balls(io, draw_list);
        draw_pad(io, draw_list);
        draw_blocks(io, draw_list);

        draw_bonuses(io, draw_list);
        draw_bonuses_message(io, draw_list);

        draw_game_play_info_ui(io, draw_list);

        break;
    }
    case ArkanoidState::Won:
    {
        draw_balls(io, draw_list);
        draw_pad(io, draw_list);
        draw_blocks(io, draw_list);

        const auto text = string_format("\tYou won!\n\tScore: %d \n\n Press SPACE to start new game", game_state.score).c_str();
        draw_text_on_white_background(draw_list, text, current_settings.world_size[0] / 2, current_settings.world_size[1] / 2);

        break;
    }
    case ArkanoidState::Lost:
    {
        draw_balls(io, draw_list);
        draw_pad(io, draw_list);
        draw_blocks(io, draw_list);

        const auto text = string_format("\tYou lost!\n\tScore: %d  \n\n Press SPACE to start new game", game_state.score).c_str();
        draw_text_on_white_background(draw_list, text, current_settings.world_size[0] / 2, current_settings.world_size[1] / 2);

        break;
    }
    }
}

void ArkanoidImpl::draw_balls(ImGuiIO& io, ImDrawList& draw_list) const
{
    for (auto& ball : balls)
    {
        const auto screen_pos = ball->transform.pos * world_space.world_to_screen;
        const auto screen_radius = ball->radius() * world_space.world_to_screen.x;

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

        const auto screen_pos = block->transform.pos * world_space.world_to_screen;

        const auto size = block->transform.size * world_space.world_to_screen;

        const auto color = block->state == BlockState::WithBonus ? ImColor(0, 255, 0) : ImColor(255, 0, 0);

        draw_list.AddRect(screen_pos, screen_pos + size, color);
    }
}

void ArkanoidImpl::draw_pad(ImGuiIO& io, ImDrawList& draw_list) const
{
    const auto screen_pos = pad->transform.pos * world_space.world_to_screen;

    const auto size = pad->transform.size * world_space.world_to_screen;

    draw_list.AddRect(screen_pos, screen_pos + size, ImColor(255, 255, 255));
}

void ArkanoidImpl::update_balls(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    for (auto& ball : balls)
    {
        ball->transform.pos += ball->velocity * elapsed;

        handle_ball_hit_edge(ball.get(), io, debug_data, elapsed);
        handle_ball_hit_blocks(ball.get(), io, debug_data, elapsed);
        handle_ball_hit_pad(ball.get(), io, debug_data, elapsed);
    }
}

void ArkanoidImpl::handle_ball_hit_edge(Ball* ball, ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    if (ball->transform.pos.x < ball->radius())
    {
        ball->transform.pos.x += (ball->radius() - ball->transform.pos.x) * 2.0f;
        ball->velocity.x *= -1.0f;

        add_debug_hit(debug_data, ball->transform.pos, Vect(1.0f, 0.0f));
    }
    else if (ball->transform.pos.x > (world_space.world_size.x - ball->radius()))
    {
        ball->transform.pos -= (ball->transform.pos.x - (world_space.world_size.x - ball->radius())) * 2.0f;
        ball->velocity.x *= -1.0f;

        add_debug_hit(debug_data, ball->transform.pos, Vect(-1.0f, 0.0f));
    }

    if (ball->transform.pos.y < ball->radius())
    {
        ball->transform.pos.y += (ball->radius() - ball->transform.pos.y) * 2.0f;
        ball->velocity.y *= -1.0f;

        add_debug_hit(debug_data, ball->transform.pos, Vect(0.0f, 1.0f));
    }
    else if (ball->transform.pos.y > (world_space.world_size.y - ball->radius()))
    {
        ball->transform.pos.y -= (ball->transform.pos.y - (world_space.world_size.y - ball->radius())) * 2.0f;
        ball->velocity.y *= -1.0f;

        add_debug_hit(debug_data, ball->transform.pos, Vect(0.0f, -1.0f));
    }
}

void ArkanoidImpl::handle_ball_hit_blocks(Ball* ball, ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    for (auto& block : blocks)
    {
        if (block->state == BlockState::Dead)
        {
            continue;
        }

        if (ball->circle_collider->check_collision_with_transform(block->transform))
        {
            auto normal = ball->transform.pos - block->transform.pos;
            normal.Normalize();

            ball->rebound_relative_to_normal(normal);

            if (block->state == BlockState::WithBonus)
            {
                spawn_random_bonus(
                    block->transform.pos.x + block->transform.size.x / 2, block->transform.pos.y + block->transform.size.y / 4);
            }

            block->state = BlockState::Dead;

            game_state.handle_destroy_block();

            add_debug_hit(debug_data, ball->transform.pos, normal);
        }
    }
}

void ArkanoidImpl::handle_ball_hit_pad(Ball* ball, ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    if (ball->circle_collider->check_collision_with_transform(pad->transform))
    {
        auto normal = pad->transform.pos - ball->transform.pos;
        normal.Normalize();

        ball->rebound_relative_to_normal(normal);

        ball->velocity.y = -abs(ball->velocity.y);

        add_debug_hit(debug_data, ball->transform.pos, normal);
    }
}

float ArkanoidImpl::calculate_brick_offset_to_center_x(const ArkanoidSettings& settings, Vect size)
{
    return world_space.world_size.x / 2.0f - (settings.bricks_columns_count * (((size.x) + settings.bricks_columns_padding) / 2.0f));
}

void ArkanoidImpl::init_blocks(const ArkanoidSettings& settings)
{
    // @Note(jejikeh): Initialize bricks only once
    if (blocks.empty())
    {
        for (int i = 0; i < settings.bricks_columns_max * settings.bricks_rows_max; i++)
        {
            blocks.push_back(std::make_unique<Block>(this, 0, 0));
        }
    }
    else
    {
        for (auto& block : blocks)
        {
            block->state = BlockState::Dead;
        }
    }

    auto block_size = settings.calculate_brick_size();

    auto center_offset_x = calculate_brick_offset_to_center_x(settings, block_size);

    for (int y = 0; y < settings.bricks_rows_count; y++)
    {
        const auto pos_y = y * block_size.y + settings.bricks_rows_padding * (y + 1);

        for (int x = 0; x < settings.bricks_columns_count; x++)
        {
            auto block = blocks.at(y * settings.bricks_columns_count + x).get();

            block->transform.pos.x = x * block_size.x + settings.bricks_columns_padding * (x + 1) + center_offset_x;
            block->transform.pos.y = pos_y;
            block->state = static_cast<BlockState>((rand() % 2) + 1);
        }
    }
}

void ArkanoidImpl::update_pad(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    if (io.KeysDown[ImGuiKey_D])
    {
        pad->target_velocity = pad->max_velocity;
    }
    else if (io.KeysDown[ImGuiKey_A])
    {
        pad->target_velocity = -pad->max_velocity;
    }
    else
    {
        pad->target_velocity = 0.0f;
    }

    pad->current_velocity = POD_LERP(pad->current_velocity, pad->target_velocity, pad->acceleration * elapsed);

    pad->transform.pos.x += pad->current_velocity * elapsed;
}

void ArkanoidImpl::add_debug_hit(ArkanoidDebugData& debug_data, const Vect& world_pos, const Vect& normal)
{
    ArkanoidDebugData::Hit hit;
    hit.screen_pos = world_pos * world_space.world_to_screen;
    hit.normal = normal;
    debug_data.hits.push_back(std::move(hit));
}

void ArkanoidImpl::draw_game_play_info_ui(ImGuiIO& io, ImDrawList& draw_list) const
{
    const auto world_size = current_settings.world_size;

    const auto text = string_format("\tScore: \t\n\t%d", game_state.score);
    const auto text_size = draw_text_on_white_background(draw_list, text.c_str(), world_size[0], world_size[1] * 0.9f);

    const auto lives_text = string_format("\tLives: \t\n\t%d", game_state.lives);
    draw_text_on_white_background(draw_list, lives_text.c_str(), world_size[0], world_size[1] - text_size.y);
};

ImVec2 ArkanoidImpl::draw_text_on_white_background(ImDrawList& draw_list, const char* text, float x, float y) const
{
    const auto text_size = ImGui::CalcTextSize(text);

    const auto pos_x = (x - text_size.x / 2) * world_space.world_to_screen.x;
    const auto pos_y = (y - text_size.y / 2) * world_space.world_to_screen.y;

    draw_list.AddRectFilled(ImVec2(pos_x, pos_y), ImVec2(pos_x + text_size.x, pos_y + text_size.y), ImColor(255, 255, 255));

    draw_list.AddText(ImVec2(pos_x, pos_y), ImColor(0, 0, 0), text);

    return text_size;
}

void ArkanoidImpl::draw_bonuses(ImGuiIO& io, ImDrawList& draw_list) const
{
    for (auto& bonus : bonuses)
    {
        bonus->draw(io, draw_list);
    }
}

void ArkanoidImpl::apply_bonus(Bonus* bonus)
{
    switch (bonus->settings->type)
    {
    case BonusType::PadSize:
    {
        pad->set_width(pad->transform.size.x - bonus->settings->value);

        break;
    }

    case BonusType::PadSpeed:
        pad->speed_modifier += bonus->settings->value;
        pad->reset(current_settings);

        break;
    case BonusType::BallSize:
    {
        for (auto& ball : balls)
        {
            ball->circle_collider->set_radius(ball.get()->transform.size.x - bonus->settings->value);
        }

        break;
    }
    case BonusType::BallSpeed:
    {
        for (auto& ball : balls)
        {
            ball->speed_modifier += bonus->settings->value;
            ball->reset(current_settings);
        }

        break;
    }
    case BonusType::AddHealth:
    {
        game_state.lives += bonus->settings->value;

        break;
    }
    case BonusType::Score:
    {
        game_state.score += bonus->settings->value;

        break;
    }
    case BonusType::NewBall:
    {
        balls.push_back(std::make_unique<Ball>(this, pad.get()->transform.pos.x, pad.get()->transform.pos.y));
        balls.back().get()->reset(current_settings);

        break;
    }
    default:
        break;
    }
}

void ArkanoidImpl::update_bonuses(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    for (auto& bonus : bonuses)
    {
        if (!bonus->enabled)
        {
            continue;
        }

        bonus->transform.pos.y += bonus->fall_speed * elapsed;

        if (bonus->cirlce_collider->check_collision_with_transform(pad->transform))
        {
            apply_bonus(bonus.get());

            auto message_text = bonus->settings->message();
            bonuse_message.get()->show(message_text);

            bonus->enabled = false;
        }
    }

    for (int i = 0; i < bonuses.size(); i++)
    {
        if (!bonuses[i]->enabled)
        {
            bonuses.erase(bonuses.begin() + i);
        }
    }
}

void ArkanoidImpl::update_bonuses_message(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    bonuse_message->update(io, debug_data, elapsed);
}

void ArkanoidImpl::draw_bonuses_message(ImGuiIO& io, ImDrawList& draw_list) const
{
    bonuse_message->draw(draw_list);
}

void ArkanoidImpl::spawn_random_bonus(int x, int y)
{
    if (!random_chance(current_settings.chance_to_spawn_brick_bonus))
    {
        return;
    }

    bonuses.push_back(std::make_unique<Bonus>(this, x, y));
    bonuses.back().get()->reset(current_settings);
}