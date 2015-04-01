#pragma once
#include <gimgui/data/rectangle.hpp>
#include <gimgui/data/renderdata.hpp>
#include <gimgui/data/rendermodes.hpp>
#include <gimgui/logic/allpropagator.hpp>
#include <gimgui/logic/absolutemap.hpp>
#include <vector>

namespace gim
{
    template <typename Vec2, typename Color>
    class RenderDataGenerator
    {
        struct FloatVec2
        {
            float x;
            float y;
        };
        public:
            RenderDataGenerator();
            std::vector<RenderData> generate(const gim::Element& element);
            uint32_t registerImageInfo(const Vec2& imageSize);
        private:
            RenderData generateElementData(const Element& element, gim::AbsoluteMap<Vec2>& absoluteMap);
            void generateQuadWithoutImage(const Vec2& position, const Vec2& size, const Color& color, std::vector<float>& outPositions, std::vector<float>& outColors);
            void generateQuadWithImage(const Vec2& position, const Vec2& size, const Color& color, const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outPositions, std::vector<float>& outColors, std::vector<float>& outTexCoords);
            void generateQuadPositions(const Vec2& position, const Vec2& size, std::vector<float>& outPositions);
            void generateQuadColors(const Color& color, std::vector<float>& outColors);
            void generateQuadTexCoords(const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outTexCoords);
            void generateBorders(const Element& element, const Vec2& position, const Vec2& size, const Color& color, const Vec2& imageSize, std::vector<float>& outPositions, std::vector<float>& outColors, std::vector<float>& outTexCoords);

            std::unordered_map<uint32_t, Vec2> mImageSizes;

            uint32_t mNextImageId;
    };

#include <gimgui/logic/renderdatagenerator.inl>
}
