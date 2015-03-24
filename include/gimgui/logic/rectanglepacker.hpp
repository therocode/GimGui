#pragma once
#include <iostream>
#include <vector>
#include <gimgui/assert.hpp>
#include <gimgui/data/rectangle.hpp>
#include <gimgui/util/guillotinebinpack.hpp>

namespace gim
{
    template <typename Vec2>
    class RectanglePacker
    {
        public:
            RectanglePacker(const int32_t size);
            Rectangle<Vec2> insert(const Vec2& size);
        private:
            Rectangle<Vec2> pack(const Vec2& size);
            int32_t indexToOrder(int32_t index);
            Vec2 indexToPosition(int32_t index);
            int32_t orderToSize(int32_t order);

            int32_t mInitialSize;
            std::vector<GuillotineBinPack> mPackers;
    };
#include <gimgui/logic/rectanglepacker.inl>
}
