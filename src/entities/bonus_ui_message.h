#pragma once

#include "entity.h"
#include "../base.h"

struct BonusUIMessage : Entity
{
    float time;
    float current_time;
    std::string text;
    bool enabled = false;

    BonusUIMessage(Arkanoid* a) : Entity(a)
    {
    }

    void show(std::string message)
    {
        this->text = message;

        enabled = true;

        fprintf(stderr, "%s\n", message.c_str());
        fprintf(stderr, "%f\n", time);

        current_time = time;
    }

    void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
    {
        if (!enabled)
        {
            return;
        }

        if (current_time <= 0.0f)
        {
            enabled = false;
        }

        printf("%f \n", current_time);

        current_time -= elapsed;
    }

    void draw(ImDrawList& draw_list) const
    {
        if (!enabled)
        {
            return;
        }

        const auto text_size = ImGui::CalcTextSize(text.c_str());

        const auto pos_x = (transform.pos.x - text_size.x / 2) * arkanoid->world_space.world_to_screen.x;
        const auto pos_y = (transform.pos.y - text_size.y / 2) * arkanoid->world_space.world_to_screen.y;

        draw_list.AddRectFilled(ImVec2(pos_x, pos_y), ImVec2(pos_x + text_size.x, pos_y + text_size.y), ImColor(255, 255, 255));

        draw_list.AddText(ImVec2(pos_x, pos_y), ImColor(0, 0, 0), text.c_str());
    }

    bool is_finished() const
    {
        return !enabled;
    }

    void reset(const ArkanoidSettings& settings)
    {
        enabled = true;
        time = settings.bonus_message_duration;

        transform.pos.x = settings.world_size[0] / 2.0f;
        transform.pos.y = settings.world_size[1] / 2.0f;
    }
};