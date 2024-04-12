#pragma once

#include "../base.h"
#include "entity.h"

enum class BonusType
{
    Random,

    PadSizeIncrease,
    PadSizeDecrease,

    PadSpeedIncrease,
    PadSpeedDecrease,

    BallSizeIncrease,
    BallSizeDecrease,

    BallSpeedIncrease,
    BallSpeedDecrease,

    AddHealth,

    Score,

    Count
};

struct BonusSettings
{
    BonusType type = BonusType::Random;
    float value = 0.0f;

    BonusSettings()
    {
        type = static_cast<BonusType>(rand() % static_cast<int>(BonusType::Count));

        switch (type)
        {
        case BonusType::PadSizeIncrease:
        case BonusType::PadSizeDecrease:
        case BonusType::BallSizeIncrease:
        case BonusType::BallSizeDecrease:
        {
            value = rand() % 10 + 1;
            break;
        }
        case BonusType::PadSpeedIncrease:
        case BonusType::PadSpeedDecrease:
        case BonusType::BallSpeedIncrease:
        case BonusType::BallSpeedDecrease:
        {
            value = rand() % 10 + 1;
            break;
        }
        case BonusType::AddHealth:
        {
            value = 1.0f;
            break;
        }
        case BonusType::Score:
        {
            value = 10.0f;
            break;
        }
        }
    };
};

struct Bonus : Entity
{
    float fall_speed = 10.0f;
    BonusSettings settings;

    Bonus(int x, int y)
    {
        transform.pos.x = x;
        transform.pos.y = y;

        transform.size = Vect(10.0f, 10.0f);

        settings = BonusSettings();
    }

    void reset(const ArkanoidSettings& settings)
    {
    }
};