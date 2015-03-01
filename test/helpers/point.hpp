#pragma once
#include <cstdint>

struct Point
{
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;

    int32_t x;
    int32_t y;
};
