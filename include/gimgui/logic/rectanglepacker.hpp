#pragma once
#include <gimgui/data/rectangle.hpp>

namespace gim
{
    template <typename Vec2>
    class RectanglePacker
    {
        public:
            RectanglePacker(const Vec2& size);
            Rectangle<Vec2> insert(const Vec2& size);
    };
#include <gimgui/logic/rectanglepacker.inl>
}
