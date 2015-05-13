#pragma once
#include <vector>
#include <memory>

namespace gim
{
    class Element;

    struct RenderData
    {
        struct ClipRect
        {
            int32_t xStart;
            int32_t yStart;
            int32_t width;
            int32_t height;
        };

        RenderData() = default;
        RenderData(const RenderData& other);
        RenderData& operator=(const RenderData& other);
        RenderData(RenderData&& other) = default;
        RenderData& operator=(RenderData&& other) = default;

        const Element* element;
        std::vector<float> positions;
        std::vector<float> colors;
        std::vector<float> texCoords;
        uint32_t imageId;

        std::vector<float> textPositions;
        std::vector<float> textColors;
        std::vector<float> textTexCoords;
        uint32_t textImageId;
        
        std::unique_ptr<ClipRect> clipRectangle;
    };
}
