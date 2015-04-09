#pragma once
#include <gimgui/data/rectangle.hpp>
#include <gimgui/data/renderdata.hpp>
#include <gimgui/data/rendermodes.hpp>
#include <gimgui/data/textstyles.hpp>
#include <gimgui/logic/allpropagator.hpp>
#include <gimgui/logic/absolutemap.hpp>
#include <gimgui/logic/fonttexturecache.hpp>
#include <gimgui/logic/utf8decoder.hpp>
#include <gimgui/util/getorfallback.hpp>
#include <vector>

namespace gim
{
    class Font;

    template <typename Vec2, typename Color>
    class RenderDataGenerator
    {
        struct FloatVec2
        {
            float x;
            float y;
        };
        struct Ids
        {
            uint32_t fontId;
            uint32_t textureId;
        };
        struct FontStorageEntry
        {
            Font& font;
            FontTextureCache textureCoordinates;
            uint32_t textureId;
            std::unordered_map<CodePointSize, Glyph::Metrics> metrics;
        };
        public:
            RenderDataGenerator();
            std::vector<RenderData> generate(const gim::Element& element);
            uint32_t registerImageInfo(const Vec2& imageSize);
            template <typename TextureAdaptor>
            Ids registerFont(Font& font, const TextureAdaptor& textureAdaptor);
        private:
            RenderData generateElementData(const Element& element, gim::AbsoluteMap<Vec2>& absoluteMap);
            void generateQuadWithoutImage(const FloatVec2& position, const FloatVec2& size, const Color& color, std::vector<float>& outPositions, std::vector<float>& outColors);
            void generateQuadWithImage(const FloatVec2& position, const FloatVec2& size, const Color& color, const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outPositions, std::vector<float>& outColors, std::vector<float>& outTexCoords, bool flipTexCoords = false);
            void generateQuadPositions(const FloatVec2& position, const FloatVec2& size, std::vector<float>& outPositions);
            void generateQuadColors(const Color& color, std::vector<float>& outColors);
            void generateQuadTexCoords(const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outTexCoords, bool flipTexCoords = false);
            void generateBorders(const Element& element, const FloatVec2& position, const FloatVec2& size, const Color& color, const FloatVec2& imageSize, std::vector<float>& outPositions, std::vector<float>& outColors, std::vector<float>& outTexCoords);

            float getHSpace(uint32_t fontId, uint32_t size);

            std::unordered_map<uint32_t, Vec2> mImageSizes;

            //fonts
            uint32_t mNextImageId;
            uint32_t mNextFontId;

            std::unordered_map<uint32_t, FontStorageEntry> mFontStorage;
    };

#include <gimgui/logic/renderdatagenerator.inl>
}
