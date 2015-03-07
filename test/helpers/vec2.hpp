#pragma once
#include <cstdint>

struct Vec2
{
    int32_t x;
    int32_t y;

    bool operator==(const Vec2& other) const;
    bool operator!=(const Vec2& other) const;
    Vec2 operator+(const Vec2& other) const;
    Vec2& operator+=(const Vec2& other);
};
