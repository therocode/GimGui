#pragma once
#include <cstdint>
#include <memory>
#include <functional>
#include <unordered_map>
#include <gimgui/data/font.hpp>
#include <gimgui/data/codepointsize.hpp>
#include <gimgui/data/texturecoordinates.hpp>
#include <gimgui/logic/rectanglepacker.hpp>

namespace gim
{
    class FontTextureCache
    {
        struct Vec2
        {
            int32_t x;
            int32_t y;
        };
        public:
            template <typename TextureAdaptor>
            FontTextureCache(TextureAdaptor textureAdaptor);
            std::unique_ptr<TextureCoordinates> glyphCoords(uint32_t codePoint, uint32_t size);
            TextureCoordinates add(const Glyph& glyph);
        private:
            std::function<void(uint32_t, uint32_t)> mResizeStorage;
            std::function<void(uint32_t, uint32_t, const BitMap&)> mWriteBitmap;

            RectanglePacker<Vec2> mGlyphPacker;
            using Rectangle = decltype(mGlyphPacker.insert(Vec2()));
            TextureCoordinates generateTexCoords(const Rectangle, bool flipped);
            std::unordered_map<CodePointSize, std::pair<Rectangle, bool>> mGlyphRectangles;
            uint32_t mCurrentSize;
    };
#include <gimgui/logic/fonttexturecache.inl>
}
