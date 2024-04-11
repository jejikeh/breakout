#pragma once

#include "render.h"

#define USE_RENDER_GL_IMPL

class RenderImplGL : public Render
{
public:
    void render_quad(Vect pos, Vect size, ImVec4 color) override;
};
