#pragma once
#include <cstdint>

struct Color
{
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    bool operator==(const Color& other) const;
    bool operator!=(const Color& other) const;

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};
