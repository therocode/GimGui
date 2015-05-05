#pragma once
#include <cstdint>
#include <vector>

namespace gim
{
    struct BitMap
    {
        uint32_t width;
        uint32_t height;
        std::vector<uint8_t> pixels;
    };
}
