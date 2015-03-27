#pragma once
#include <gimgui/data/bitmap.hpp>

namespace gim
{
    struct Glyph
    {
        float advance;
        BitMap image;
    };
}
