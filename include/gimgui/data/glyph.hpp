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
            float top;
            float width;
            float height;
        };

        uint32_t codePoint;
        uint32_t size;
        BitMap image;
        Metrics metrics;
    };
}
