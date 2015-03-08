#pragma once
#include <gimgui/data/rectangle.hpp>
#include <gimgui/data/renderdata.hpp>
#include <gimgui/logic/allpropagator.hpp>
#include <gimgui/logic/absolutemap.hpp>
#include <vector>

namespace gim
{
    template <typename Vec2, typename Color>
    class RenderDataGenerator
    {
        public:
            enum class StretchMode {STRETCHED, TILED, H_TILED, V_TILED};
            //"stretch_mode":StretchMode;

            enum class BorderMode {NONE, LEFT_RIGHT, TOP_BOTTOM, FULL};
            //"border_mode":BorderMode;
            //"border_coords_tl":Rectangle<Vec2>(start, size);
            //"border_coords_t": Rectangle<Vec2>(start, size);
            //"border_coords_tr":Rectangle<Vec2>(start, size);
            //"border_coords_r": Rectangle<Vec2>(start, size);
            //"border_coords_br":Rectangle<Vec2>(start, size);
            //"border_coords_b": Rectangle<Vec2>(start, size);
            //"border_coords_bl":Rectangle<Vec2>(start, size);
            //"border_coords_l": Rectangle<Vec2>(start, size);

            std::vector<RenderData> generate(const gim::Element& element);
            void registerImageInfo(int32_t imageId, const Vec2& imageSize);
        private:
            void generateQuadPositions(const Vec2& position, const Vec2& size, std::vector<float>& outPositions);
            void generateQuadColors(const Color& color, std::vector<float>& outColors);
            void generateQuadTexCoords(const std::array<float, 2>& texCoordStart, const std::array<float, 2>& texCoordSize, std::vector<float>& outTexCoords);

            std::unordered_map<int32_t, Vec2> mImageSizes;
    };

#include <gimgui/logic/renderdatagenerator.inl>
}
