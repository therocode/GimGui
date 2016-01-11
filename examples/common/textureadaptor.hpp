#pragma once
#include <glutils/texture.hpp>
#include <gimgui/data/bitmap.hpp>

class TextureAdaptor
{
    public:
        TextureAdaptor(Texture& texture);
        void initialize(uint32_t width, uint32_t height);
        void resize(uint32_t width, uint32_t height);
        void writeBitmap(uint32_t x, uint32_t y, const gim::BitMap& bitMap);
        uint32_t handle() const;
    private:
        Texture& mTexture;
};
