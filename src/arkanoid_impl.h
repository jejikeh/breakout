#pragma once

#include "arkanoid.h"
#include "entities/ball.h"
#include "entities/block.h"
#include "entities/pad.h"

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
    int score = 0;
    int lives = 3;
    ArkanoidState state = ArkanoidState::Waiting;

    ArkanoidGameState() = default;
};

class ArkanoidImpl : public Arkanoid
{
public:
    ArkanoidImpl();

    void reset(const ArkanoidSettings& settings) override;
    void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) override;
    void draw(ImGuiIO& io, ImDrawList& draw_list) override;

    ArkanoidSettings current_settings;

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

    // UI Stufd
    void draw_game_play_info_ui(ImGuiIO& io, ImDrawList& draw_list) const;

    ImVec2 draw_text_on_white_background(ImDrawList& draw_list, const char* text, float x, float y) const;
};
