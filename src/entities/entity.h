#pragma once

#include "components.h"

struct Entity
{
    static WorldSpace world_space;

    Transform transform;

    Entity() = default;

    virtual void reset(const ArkanoidSettings& settings) = 0;
    virtual void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) = 0;
    virtual void draw(ImGuiIO& io, ImDrawList& draw_list) = 0;
};