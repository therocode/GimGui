#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <gimgui/assert.hpp>
#include <gimgui/data/rectangle.hpp>
#include <gimgui/util/guillotinebinpack.hpp>

namespace gim
{
    namespace internal
    {
            class RectanglePacker
            {
                public:
                    RectanglePacker(const int32_t size);
                    Rectangle insert(const IVec2& size);
                private:
                    Rectangle pack(const IVec2& size);
                    int32_t indexToOrder(int32_t index);
                    IVec2 indexToPosition(int32_t index);
                    int32_t orderToSize(int32_t order);

                    int32_t mInitialSize;
                    std::vector<GuillotineBinPack> mPackers;
            };
    }
}
