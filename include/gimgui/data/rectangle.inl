#pragma once
#include <gimgui/data/rectangle.hpp>

template <typename Vec2>
Rectangle<Vec2>::Rectangle(const Vec2& startParameter, const Vec2& sizeParameter):
    start(startParameter),
    size(sizeParameter)
{
}
