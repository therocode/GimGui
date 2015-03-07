#include <helpers/color.hpp>

bool Color::operator==(const Color& other) const
{
    return r == other.r && g == other.g && b == other.b;
}

bool Color::operator!=(const Color& other) const
{
    return !(other == *this);
}
