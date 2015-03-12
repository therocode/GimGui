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
        int32_t imageId;
    };
}
