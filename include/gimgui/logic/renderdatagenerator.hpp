#pragma once
#include <gimgui/data/rectangle.hpp>
#include <gimgui/data/renderdata.hpp>
#include <gimgui/data/rendermodes.hpp>
#include <gimgui/data/textstyles.hpp>
#include <gimgui/data/textalignments.hpp>
#include <gimgui/data/wrapmode.hpp>
#include <gimgui/data/codepointsize.hpp>
#include <gimgui/logic/foreach.hpp>
#include <gimgui/logic/fonttexturecache.hpp>
#include <gimgui/logic/utf8decoder.hpp>
#include <gimgui/util/getorfallback.hpp>
#include <gimgui/util/ref.hpp>
#include <vector>
#include <memory>

namespace gim
{
    class Font;

    template <typename Vec2, typename Rectangle, typename Color, typename Texture>
    class RenderDataGenerator
    {
        using MetricsMap = std::unordered_map<CodePointSize, Glyph::Metrics>;
        struct FloatVec2
        {
            float x;
            float y;
        };
        struct FontCacheEntry
        {
            internal::FontTextureCache textureCoordinates;
            //uint32_t textureId;
            std::unique_ptr<Texture> texture;
            std::unordered_map<std::string, int32_t> fontIndices;
            std::vector<MetricsMap> metrics;
        };
        public:
            //RenderDataGenerator();
            std::vector<RenderData<Texture>> generate(const gim::Element& element);
            void registerTexture(const std::string& textureName, Texture texture);
            void registerFontStorage(const std::vector<std::reference_wrapper<Font>>& fonts, Texture texture);
        private:
            RenderData<Texture> generateElementData(const Element& element, const Vec2& absolutePosition);
            void generateQuadWithoutImage(const FloatVec2& position, const FloatVec2& size, const Color& color, float zPosition, std::vector<float>& outPositions, std::vector<float>& outColors);
            void generateQuadWithImage(const FloatVec2& position, const FloatVec2& size, const Color& color, float zPosition, const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outPositions, std::vector<float>& outColors, std::vector<float>& outTexCoords, bool flipTexCoords = false);
            void generateQuadPositions(const FloatVec2& position, const FloatVec2& size, float zPosition, std::vector<float>& outPositions);
            void generateQuadColors(const Color& color, std::vector<float>& outColors);
            void generateQuadTexCoords(const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outTexCoords, bool flipTexCoords = false);
            void generateBorders(const Element& element, const FloatVec2& position, const FloatVec2& size, const Color& color, float zPosition, const FloatVec2& imageSize, std::vector<float>& outPositions, std::vector<float>& outColors, std::vector<float>& outTexCoords);

            float getHSpace(const Font& font, uint32_t size);
            std::unique_ptr<std::tuple<TextureCoordinates, Glyph::Metrics>> loadGlyphData(int32_t fontId, uint32_t codePoint, uint32_t textSize, internal::FontTextureCache& textureCache, MetricsMap& metricsMap, const Font& font);

            std::unordered_map<std::string, Texture> mTextures;

            //std::unordered_map<std::string, uint32_t> mFontCacheIds;
            //std::unordered_map<uint32_t, std::shared_ptr<FontCacheEntry>> mFontCache;
            std::unordered_map<std::string, std::shared_ptr<FontCacheEntry>> mFontCache;
            std::unordered_map<std::string, std::reference_wrapper<Font>> mFonts;
    };

#include <gimgui/logic/renderdatagenerator.inl>
}
