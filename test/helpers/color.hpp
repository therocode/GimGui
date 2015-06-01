#pragma once
#include <cstdint>

struct Color
{
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    bool operator==(const Color& other) const;
    bool operator!=(const Color& other) const;

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

class ColorAdaptor
{
    public:
        using Native = Color;
        ColorAdaptor();
        ColorAdaptor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
        ColorAdaptor(const Color& color);
        uint8_t r() const;
        uint8_t g() const;
        uint8_t b() const;
        uint8_t a() const;
    private:
        Color mColor;
};
