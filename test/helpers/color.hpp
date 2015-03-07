#pragma once
#include <cstdint>

struct Color
{
    bool operator==(const Color& other) const;
    bool operator!=(const Color& other) const;

    uint8_t r;
    uint8_t g;
    uint8_t b;
};
