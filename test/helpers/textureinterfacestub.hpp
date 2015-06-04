#pragma once
#include <cstdint>
#include <gimgui/data/bitmap.hpp>
#include <helpers/vec2.hpp>

class TextureInterfaceStub
{
    public:
        TextureInterfaceStub(const Vec2& size);
        void initialize(uint32_t width, uint32_t height);
        void resize(uint32_t width, uint32_t height);
        void writeBitmap(uint32_t x, uint32_t y, const gim::BitMap& bitMap);
        //Vec2 size() const;
    private:
        Vec2 mSize;
};
