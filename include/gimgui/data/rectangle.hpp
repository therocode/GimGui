#pragma once

namespace gim
{
    template<typename Vec2>
    struct Rectangle
    {
        Rectangle(const Vec2& startParameter, const Vec2& sizeParameter);
        Vec2 start;
        Vec2 size;
    };

#include <gimgui/data/rectangle.inl>
}
