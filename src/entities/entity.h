#pragma once

#include "components.h"

struct Entity
{
    // @Incomplete(jejikeh): Delete thattt!!
    static WorldSpace world_space;

    Transform transform;

    Entity() = default;

    virtual void reset(const ArkanoidSettings& settings) = 0;

    // @Note(jejikeh): I want keep things realy simple, so
    // i decide to not add some complex logic to Entities.
    // Because this leads to more complex entity managment.

    // void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) override;

    // @Note(jejikeh): Since draw() shouldnt contatin any complex logic,
    // I think is fine to keep it simple.
    // But i dont realy like the separation of logic and rendering. Maybe it is better to move drawing also to ArkanoidImpl.

    // virtual void draw(ImGuiIO& io, ImDrawList& draw_list) = 0;
};