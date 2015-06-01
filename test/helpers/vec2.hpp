#pragma once
#include <cstdint>

struct Vec2
{
    int32_t x;
    int32_t y;

    bool operator==(const Vec2& other) const;
    bool operator!=(const Vec2& other) const;
    Vec2 operator+(const Vec2& other) const;
    Vec2& operator+=(const Vec2& other);
};

class Vec2Adaptor
{
    public:
        using Native = Vec2;
        Vec2Adaptor();
        Vec2Adaptor(int32_t x, int32_t y);
        Vec2Adaptor(const Vec2& vec2);
        Vec2Adaptor operator+(const Vec2Adaptor& other) const;
        int32_t x() const;
        int32_t y() const;
        void x(int32_t x);
        void y(int32_t y);
    private:
        Vec2 mVec2;
};
