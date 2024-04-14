#pragma once

#include "base.h"
#include "entities/components.h"
#include <vector>

struct ArkanoidSettings
{
    static constexpr int bricks_columns_min = 10;
    static constexpr int bricks_columns_max = 30;
    static constexpr int bricks_rows_min = 3;
    static constexpr int bricks_rows_max = 10;

    static constexpr float bricks_columns_padding_min = 5.0f;
    static constexpr float bricks_columns_padding_max = 20.0f;
    static constexpr float bricks_rows_padding_min = 5.0f;
    static constexpr float bricks_rows_padding_max = 20.0f;

    static constexpr float bricks_height_min = 1.0f;
    static constexpr float bricks_height_max = 10.0f;

    static constexpr float ball_radius_min = 5.0f;
    static constexpr float ball_radius_max = 50.0f;
    static constexpr float ball_speed_min = 1.0f;
    static constexpr float ball_speed_max = 1000.0f;

    static constexpr float carriage_speed_max = 500.0f;
    static constexpr float carriage_acceleration_max = 2.0f;
    static constexpr float carriage_acceleration_min = 20.0f;

    static constexpr float bonus_radius_min = 1.0f;
    static constexpr float bonus_radius_max = 10.0f;
    static constexpr float bonus_fall_speed_max = 100.0f;
    static constexpr float bonus_fall_speed_min = 1.0f;

    static constexpr float carriage_width_min = 50.0f;
    static constexpr float carriage_width_max = 200.0f;

    static constexpr float bonus_message_duration_min = 1.0f;
    static constexpr float bonus_message_duration_max = 10.0f;

    static constexpr float chance_to_spawn_brick_bonus_min = 0.01f;
    static constexpr float chance_to_spawn_brick_bonus_max = 1.0f;

    static constexpr float bonus_pad_size_modifier_max = 10.0f;
    static constexpr float bonus_pad_size_modifier_min = 0.1f;

    Vect world_size = Vect(800.0f, 600.f);

    int bricks_columns_count = 15;
    int bricks_rows_count = 7;

    float bricks_columns_padding = 5.0f;
    float bricks_rows_padding = 5.0f;

    float bricks_height = 3.0f;

    float ball_radius = 10.0f;
    float ball_speed = 200.0f;

    float carriage_speed = 200.0f;
    float carriage_acceleration = 10.0f;
    float carriage_width = 100.0f;

    float bonus_radius = 5.0f;
    float bonus_fall_speed = 50.0f;

    float bonus_message_duration = 2.0f;

    float chance_to_spawn_brick_bonus = 0.1f;

    float bonus_pad_size_modifier = 0.1f;

    Vect calculate_brick_size() const
    {
        return Vect(
            world_size.x / bricks_columns_count - bricks_columns_padding,
            (world_size.y / bricks_rows_count - bricks_rows_padding) / bricks_height);
    }
};

struct ArkanoidDebugData
{
    struct Hit
    {
        Vect screen_pos;   // Hit position, in screen space
        Vect normal;       // Hit normal
        float time = 0.0f; // leave it default
    };

    std::vector<Hit> hits;
};

class Arkanoid
{
public:
    WorldSpace world_space;
    ArkanoidSettings current_settings;

    virtual ~Arkanoid() = default;
    virtual void reset(const ArkanoidSettings& settings) = 0;
    virtual void draw(ImGuiIO& io, ImDrawList& draw_list) = 0;
    virtual void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) = 0;
};

extern Arkanoid* create_arkanoid();
