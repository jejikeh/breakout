#pragma once

#include "arkanoid.h"
#include "entities/ball.h"
#include "entities/block.h"

#define USE_ARKANOID_IMPL

class ArkanoidImpl : public Arkanoid
{
public:
    ArkanoidImpl();

    void reset(const ArkanoidSettings& settings) override;
    void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) override;
    void draw(ImGuiIO& io, ImDrawList& draw_list) override;

private:
    // Enitities
    std::vector<Ball*> balls;

    void draw_balls(ImGuiIO& io, ImDrawList& draw_list);
    void update_balls(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);
    void update_balls_collision(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);

    std::vector<Block*> blocks;

    void init_blocks(const ArkanoidSettings& settings);
    void draw_blocks(ImGuiIO& io, ImDrawList& draw_list) const;
    void update_blocks(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);

    void add_debug_hit(ArkanoidDebugData& debug_data, const Vect& world_pos, const Vect& normal);

    float calculate_offset_to_center(const ArkanoidSettings& settings, Vect block_size);
};
