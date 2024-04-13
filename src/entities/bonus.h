#pragma once

#include "../base.h"
#include "entity.h"

#define PAD_SIZE_MAX_MODIFIER 10
#define PAD_SPEED_MAX_MODIFIER 2
#define BALL_SIZE_MAX_MODIFIER 2
#define BALL_SPEED_MAX_MODIFIER 2
#define ADD_HEALTH_MAX_MODIFIER 3
#define SCORE_MAX_MODIFIER 100
#define NEW_BALL_MAX_MODIFIER 3

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

    BonusSettings()
    {
        type = static_cast<BonusType>(rand() % static_cast<int>(BonusType::Count));
        // type = BonusType::BallSize;

        switch (type)
        {
        case BonusType::PadSize:
        {
            value = random_value(PAD_SIZE_MAX_MODIFIER);

            break;
        }
        case BonusType::PadSpeed:
        {
            value = random_value(PAD_SPEED_MAX_MODIFIER);

            break;
        }
        case BonusType::BallSize:
        {
            value = random_value(BALL_SIZE_MAX_MODIFIER);

            break;
        }
        case BonusType::BallSpeed:
        {
            value = random_value(BALL_SPEED_MAX_MODIFIER);

            break;
        }
        case BonusType::AddHealth:
        {
            value = random_value(ADD_HEALTH_MAX_MODIFIER);

            break;
        }
        case BonusType::Score:
        {
            value = random_value(SCORE_MAX_MODIFIER);

            break;
        }
        case BonusType::NewBall:
        {
            value = random_value(NEW_BALL_MAX_MODIFIER);

            break;
        }
        }

        // value *= rand_r((unsigned int*)1) == 0 ? -1 : 1;
    };

private:
    static inline float random_value(int max)
    {
        return rand() % max + 1;
    }
};

struct Bonus : Entity
{
    CircleCollider* cirlce_collider;

    float fall_speed;
    BonusSettings settings;

    bool enabled = true;

    Bonus(int x, int y)
    {
        transform.pos.x = x;
        transform.pos.y = y;

        settings = BonusSettings();
        cirlce_collider = new CircleCollider(&transform);
    }

    void reset(const ArkanoidSettings& settings)
    {
        fall_speed = settings.bonus_fall_speed;

        // transform.size.y = settings.calculate_brick_size().y / 4;
        // transform.size.x = transform.size.y;

        cirlce_collider->set_radius(settings.bonus_radius);
    }
};