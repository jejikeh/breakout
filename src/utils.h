#pragma once

#include "base.h"

#define POD_LERP(a, b, t) (a * (1.0f - t) + b * t)

#define DOT(a, b) (a.x * b.x + a.y * b.y)

template <typename... Args> std::string string_format(const std::string& format, Args... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (size_s <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

static inline float random_float(float x)
{
    return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / x));
};

static inline float random_int(int max)
{
    return rand() % max + 1;
}

static inline bool random_chance(double p)
{
    return rand() / (RAND_MAX + 1.0) < p;
}