#pragma once
#include <glutils/texture.hpp>
#include <gimgui/data/bitmap.hpp>
#include <helpers/vector2.hpp>

class TextureAdaptor
{
    public:
        TextureAdaptor(Texture& texture);
        void initialize(uint32_t width, uint32_t height);
        void resize(uint32_t width, uint32_t height);
        void writeBitmap(uint32_t x, uint32_t y, const gim::BitMap& bitMap);
        uint32_t handle() const;
        Vec2Adaptor size() const;
        const Texture& internal() const;
    private:
        std::reference_wrapper<Texture> mTexture;
};
