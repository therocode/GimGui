#include <helpers/color.hpp>

Color::Color():
    r(255), g(255), b(255), a(255)
{
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
    r(r), g(g), b(b), a(a)
{
}

bool Color::operator==(const Color& other) const
{
    return r == other.r && g == other.g && b == other.b && a == other.a;
}

bool Color::operator!=(const Color& other) const
{
    return !(other == *this);
}
