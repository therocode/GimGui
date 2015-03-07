#pragma once

struct Vec2
{
    int32_t x;
    int32_t y;

    Vec2 operator+(const Vec2& other)
    {
        x += other.x;
        y += other.y;

        return *this;
    }
};
