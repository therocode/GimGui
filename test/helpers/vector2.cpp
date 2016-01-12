#include <helpers/vector2.hpp>

Vector2 Vector2::operator+(const Vector2& other) const
{
    Vector2 result;

    result.x = x + other.x;
    result.y = y + other.y;

    return result;
}

Vector2& Vector2::operator+=(const Vector2& other)
{
    x += other.x;
    y += other.y;

    return *this;
}

bool Vector2::operator==(const Vector2& other) const
{
    return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2& other) const
{
    return !(other == *this);
}

Vec2Adaptor::Vec2Adaptor()
{
}

Vec2Adaptor::Vec2Adaptor(int32_t x, int32_t y):
    mVec2({x, y})
{
}

Vec2Adaptor::Vec2Adaptor(const Vector2& vec2):
    mVec2(vec2)
{
}

Vec2Adaptor Vec2Adaptor::operator+(const Vec2Adaptor& other) const
{
    return mVec2 + other.mVec2;
}

int32_t Vec2Adaptor::x() const
{
    return mVec2.x;
}

int32_t Vec2Adaptor::y() const
{
    return mVec2.y;
}

void Vec2Adaptor::x(int32_t x)
{
    mVec2.x = x;
}

void Vec2Adaptor::y(int32_t y)
{
    mVec2.y = y;
}
