#pragma once
#include <cstdint>

struct Vector2
{
    int32_t x;
    int32_t y;

    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;
    Vector2 operator+(const Vector2& other) const;
    Vector2& operator+=(const Vector2& other);
};

class Vec2Adaptor
{
    public:
        using Native = Vector2;
        Vec2Adaptor();
        Vec2Adaptor(int32_t x, int32_t y);
        Vec2Adaptor(const Vector2& vec2);
        Vec2Adaptor operator+(const Vec2Adaptor& other) const;
        int32_t x() const;
        int32_t y() const;
        void x(int32_t x);
        void y(int32_t y);
    private:
        Vector2 mVec2;
};
