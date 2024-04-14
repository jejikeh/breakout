#pragma once

#include "../base.h"
#include "../utils.h"
#include "entity.h"

#define PAD_SPEED_MAX_MODIFIER 2
#define BALL_SIZE_MAX_MODIFIER 2
#define BALL_SPEED_MAX_MODIFIER 2
#define ADD_HEALTH_MAX_MODIFIER 3
#define SCORE_MAX_MODIFIER 100
#define NEW_BALL_MAX_MODIFIER 3

#define BONUS_BLINK_ANIMATION_ON_TIME 2.5f
#define BONUS_BLINK_ANIMTATION_OFF_TIME 5.0f

enum class BonusType
{
    PadSize,
    PadSpeed,
    BallSize,
    BallSpeed,
    AddHealth,
    Score,
    NewBall,
    Count
};

struct BonusSettings
{
    float value = 0.0f;
    BonusType type;

    BonusSettings(ArkanoidSettings& settings)
    {
        type = static_cast<BonusType>(rand() % static_cast<int>(BonusType::Count));
        // type = BonusType::BallSize;

        switch (type)
        {
        case BonusType::PadSize:
        {
            value = random_float(settings.bonus_pad_size_modifier);

            break;
        }
        case BonusType::PadSpeed:
        {
            value = random_float(PAD_SPEED_MAX_MODIFIER);

            break;
        }
        case BonusType::BallSize:
        {
            value = random_float(BALL_SIZE_MAX_MODIFIER);

            break;
        }
        case BonusType::BallSpeed:
        {
            value = random_float(BALL_SPEED_MAX_MODIFIER);

            break;
        }
        case BonusType::AddHealth:
        {
            value = random_int(ADD_HEALTH_MAX_MODIFIER);

            break;
        }
        case BonusType::Score:
        {
            value = random_int(SCORE_MAX_MODIFIER);

            break;
        }
        case BonusType::NewBall:
        {
            value = random_int(NEW_BALL_MAX_MODIFIER);

            break;
        }
        }
    };

    std::string message()
    {
        switch (type)
        {
        case BonusType::PadSize:
        {
            return "Pad size bonus " + std::to_string(value);
        }
        case BonusType::PadSpeed:
        {
            return "Pad speed bonus " + std::to_string(value);
        }
        case BonusType::BallSize:
        {
            return "Ball size bonus " + std::to_string(value);
        }
        case BonusType::BallSpeed:
        {
            return "Ball speed bonus " + std::to_string(value);
        }
        case BonusType::AddHealth:
        {
            return "Health bonus " + std::to_string(value);
        }
        case BonusType::Score:
        {
            return "Score bonus " + std::to_string(value);
        }
        }

        return "Bonus " + std::to_string(value);
    }
};

struct Bonus : Entity
{
    CircleCollider* cirlce_collider;
    BonusSettings* settings;

    float fall_speed;

    bool enabled = true;

    Bonus(Arkanoid* a, int x, int y) : Entity(a)
    {
        transform.pos.x = x;
        transform.pos.y = y;

        settings = new BonusSettings(a->current_settings);

        cirlce_collider = new CircleCollider(&transform);
    }

    void draw(ImGuiIO& io, ImDrawList& draw_list)
    {
        static float t_time = 0.0f;
        t_time += 0.1f;

        if (!enabled)
        {
            return;
        }

        const auto screen_pos = transform.pos * arkanoid->world_space.world_to_screen;
        const auto size = cirlce_collider->radius * arkanoid->world_space.world_to_screen;

        if (t_time < BONUS_BLINK_ANIMATION_ON_TIME)
        {
            draw_list.AddCircle(screen_pos, size.Length(), ImColor(255, 255, 0));
        }
        else if (t_time > BONUS_BLINK_ANIMATION_ON_TIME && t_time < BONUS_BLINK_ANIMTATION_OFF_TIME)
        {
            draw_list.AddCircleFilled(screen_pos, size.Length(), ImColor(255, 255, 0));
        }
        else
        {
            t_time = 0.0f;
        }
    }

    void reset(const ArkanoidSettings& settings)
    {
        fall_speed = settings.bonus_fall_speed;
        cirlce_collider->set_radius(settings.bonus_radius);
    }
};