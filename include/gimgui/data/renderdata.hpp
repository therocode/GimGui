#pragma once
#include <vector>

namespace gim
{
    class Element;

    struct RenderData
    {
        const Element* element;
        std::vector<float> positions;
        std::vector<float> colors;
        std::vector<float> texCoords;
        uint32_t imageId;

        std::vector<float> textPositions;
        std::vector<float> textColors;
        std::vector<float> textTexCoords;
        uint32_t textImageId;
    };
}
