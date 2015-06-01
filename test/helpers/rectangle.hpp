#pragma once
#include <cstdint>
#include <helpers/vec2.hpp>

struct Rectangle
{
    Vec2 start;
    Vec2 size;

    bool operator==(const Rectangle& other) const;
    bool operator!=(const Rectangle& other) const;
};

class RectangleAdaptor
{
    public:
        using Native = Rectangle;
        RectangleAdaptor();
        RectangleAdaptor(const Vec2Adaptor& start, const Vec2Adaptor& size);
        RectangleAdaptor(const Rectangle& rectangle);
        Vec2Adaptor start() const;
        Vec2Adaptor size() const;
    private:
        Rectangle mRectangle;
};
