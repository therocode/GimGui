#pragma once
#include <cstdint>

struct Vec2
{
    int32_t x;
    int32_t y;

    Vec2 operator+(const Vec2& other) const
    {
        Vec2 result;

        result.x = x + other.x;
        result.y = y + other.y;

        return result;
    }

    Vec2& operator+=(const Vec2& other)
    {
        x += other.x;
        y += other.y;

        return *this;
    }
};
