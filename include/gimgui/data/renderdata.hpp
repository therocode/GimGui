#pragma once
#include <vector>

struct RenderData
{
    std::vector<float> positions;
    std::vector<float> colors;
    std::vector<float> texCoords;
    int32_t imageId;
};
