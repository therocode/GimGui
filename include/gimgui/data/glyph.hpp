#pragma once
#include <gimgui/data/bitmap.hpp>

namespace gim
{
    struct Glyph
    {
        struct Metrics
        {
            float advance;
            float left;
            float right;
            float top;
            float bottom;
        };

        uint32_t codePoint;
        uint32_t size;
        BitMap image;
        Metrics metrics;
    };
}
