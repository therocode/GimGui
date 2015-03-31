#pragma once
#include <cstdint>
#include <gimgui/data/bitmap.hpp>

class TextureInterfaceStub
{
    public:
        void initialize(uint32_t width, uint32_t height);
        void resize(uint32_t width, uint32_t height);
        void writeBitmap(uint32_t x, uint32_t y, const gim::BitMap& bitMap);
};
