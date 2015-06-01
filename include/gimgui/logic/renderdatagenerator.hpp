#pragma once
#include <gimgui/data/rectangle.hpp>
#include <gimgui/data/renderdata.hpp>
#include <gimgui/data/rendermodes.hpp>
#include <gimgui/data/textstyles.hpp>
#include <gimgui/data/textalignments.hpp>
#include <gimgui/data/wrapmode.hpp>
#include <gimgui/logic/allpropagator.hpp>
#include <gimgui/logic/absolutemap.hpp>
#include <gimgui/logic/fonttexturecache.hpp>
#include <gimgui/logic/utf8decoder.hpp>
#include <gimgui/util/getorfallback.hpp>
#include <gimgui/util/ref.hpp>
#include <vector>
#include <memory>

namespace gim
{
    class Font;

    template <typename Vec2, typename Rectangle, typename Color>
    class RenderDataGenerator
    {
        using MetricsMap = std::unordered_map<CodePointSizeId, Glyph::Metrics>;
        struct FloatVec2
        {
            float x;
            float y;
        };
        struct FontCacheEntry
        {
            internal::FontTextureCache textureCoordinates;
            uint32_t textureId;
            MetricsMap metrics;
        };
        public:
            RenderDataGenerator();
            std::vector<RenderData> generate(const gim::Element& element);
            template <typename Texture>
            uint32_t registerTexture(const Texture& texture);
            template <typename Texture>
            uint32_t registerFontStorage(const std::vector<std::reference_wrapper<const Font>>& fonts, const Texture& texture);
        private:
            RenderData generateElementData(const Element& element, gim::AbsoluteMap<typename Vec2::Native>& absoluteMap);
            void generateQuadWithoutImage(const FloatVec2& position, const FloatVec2& size, const Color& color, std::vector<float>& outPositions, std::vector<float>& outColors);
            void generateQuadWithImage(const FloatVec2& position, const FloatVec2& size, const Color& color, const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outPositions, std::vector<float>& outColors, std::vector<float>& outTexCoords, bool flipTexCoords = false);
            void generateQuadPositions(const FloatVec2& position, const FloatVec2& size, std::vector<float>& outPositions);
            void generateQuadColors(const Color& color, std::vector<float>& outColors);
            void generateQuadTexCoords(const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outTexCoords, bool flipTexCoords = false);
            void generateBorders(const Element& element, const FloatVec2& position, const FloatVec2& size, const Color& color, const FloatVec2& imageSize, std::vector<float>& outPositions, std::vector<float>& outColors, std::vector<float>& outTexCoords);

            float getHSpace(const Font& font, uint32_t size);
            std::unique_ptr<std::tuple<TextureCoordinates, Glyph::Metrics>> loadGlyphData(uint32_t codePoint, uint32_t textSize, uint32_t fontCacheId, internal::FontTextureCache& textureCache, MetricsMap& metricsMap, const Font& font);

            std::unordered_map<uint32_t, Vec2> mTextureSizes;

            //fonts
            uint32_t mNextTextureId;
            uint32_t mNextFontId;

            std::unordered_map<std::string, uint32_t> mFontCacheIds;
            std::unordered_map<uint32_t, std::shared_ptr<FontCacheEntry>> mFontCache;
    };

#include <gimgui/logic/renderdatagenerator.inl>
}
