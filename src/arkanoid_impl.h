#pragma once

#include "arkanoid.h"
#include "entities/ball.h"
#include "entities/block.h"
#include "entities/pad.h"
#include "entities/bonus.h"
#include "entities/bonus_ui_message.h"

#define USE_ARKANOID_IMPL

enum class ArkanoidState
{
    Waiting,
    Playing,
    Won,
    Lost
};

struct ArkanoidGameState
{
    int blocks_destroyed = 0;

    int score = 0;
    int high_score = 0;

    int lives = 3;

    ArkanoidState state = ArkanoidState::Waiting;

    ArkanoidGameState() = default;

    void handle_destroy_block()
    {
        blocks_destroyed++;
        score += 10;
    }

    void reset()
    {
        if (score > high_score)
        {
            high_score = score;
        }

        blocks_destroyed = 0;
        score = 0;
        lives = 3;
    }

    void decrement_lives()
    {
        lives--;

        if (lives <= 0)
        {
            state = ArkanoidState::Lost;
        }
    }
};

class ArkanoidImpl : public Arkanoid
{
public:
    ArkanoidImpl();

    void reset(const ArkanoidSettings& settings) override;
    void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) override;
    void draw(ImGuiIO& io, ImDrawList& draw_list) override;

private:
    ArkanoidGameState game_state;

    // Balls
    std::vector<std::unique_ptr<Ball>> balls;

    void draw_balls(ImGuiIO& io, ImDrawList& draw_list) const;
    void update_balls(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);

    void handle_ball_hit_edge(Ball* ball, ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);
    void handle_ball_hit_blocks(Ball* ball, ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);
    void handle_ball_hit_pad(Ball* ball, ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);

    // Blocks
    std::vector<std::unique_ptr<Block>> blocks;

    void init_blocks(const ArkanoidSettings& settings);
    void draw_blocks(ImGuiIO& io, ImDrawList& draw_list) const;
    void update_blocks(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);
    float calculate_brick_offset_to_center_x(const ArkanoidSettings& settings, Vect block_size);

    // Pad
    std::unique_ptr<Pad> pad;

    void draw_pad(ImGuiIO& io, ImDrawList& draw_list) const;
    void update_pad(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);

    void add_debug_hit(ArkanoidDebugData& debug_data, const Vect& world_pos, const Vect& normal);

    // Bonuses
    std::vector<std::unique_ptr<Bonus>> bonuses;

    void draw_bonuses(ImGuiIO& io, ImDrawList& draw_list) const;
    void update_bonuses(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);
    void spawn_random_bonus(int x, int y);
    void apply_bonus(Bonus* bonus);

    // UI Stufd
    std::unique_ptr<BonusUIMessage> bonuse_message;

    void draw_bonuses_message(ImGuiIO& io, ImDrawList& draw_list) const;
    void update_bonuses_message(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);

    void draw_game_play_info_ui(ImGuiIO& io, ImDrawList& draw_list) const;
    ImVec2 draw_text_on_white_background(ImDrawList& draw_list, const char* text, float x, float y) const;
};
