#pragma once

#include "arkanoid.h"
#include "entities/ball.h"

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
    std::vector<Ball> balls;
};
