#pragma once
#include <cstdint>
#include <gimgui/data/bitmap.hpp>
#include <helpers/vector2.hpp>

class TextureInterfaceStub
{
    public:
        TextureInterfaceStub(const Vector2& size, uint32_t handle);
        void initialize(uint32_t width, uint32_t height);
        void resize(uint32_t width, uint32_t height);
        void writeBitmap(uint32_t x, uint32_t y, const gim::BitMap& bitMap);
        Vector2 size() const;
        uint32_t handle() const;
    private:
        Vector2 mSize;
        uint32_t mHandle;
};
