#include <helpers/vec2.hpp>

Vec2 Vec2::operator+(const Vec2& other) const
{
    Vec2 result;

    result.x = x + other.x;
    result.y = y + other.y;

    return result;
}

Vec2& Vec2::operator+=(const Vec2& other)
{
    x += other.x;
    y += other.y;

    return *this;
}

bool Vec2::operator==(const Vec2& other) const
{
    return x == other.x && y == other.y;
}

bool Vec2::operator!=(const Vec2& other) const
{
    return !(other == *this);
}
