#pragma once

#include "base.h"

class Render
{
public:
    virtual ~Render() = default;

    virtual void render_quad(Vect pos, Vect size, ImVec4 color) = 0;
};