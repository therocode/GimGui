#pragma once
#include <gimgui/data/bitmap.hpp>

namespace gim
{
    struct Glyph
    {
        uint32_t codePoint;
        uint32_t size;
        float advance;
        BitMap image;
    };
}
