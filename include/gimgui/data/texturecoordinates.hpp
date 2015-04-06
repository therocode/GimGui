#pragma once

namespace gim
{
    struct TextureCoordinates
    {
        bool operator==(const TextureCoordinates& other);
        float xStart;
        float yStart;
        float xEnd;
        float yEnd;
        bool flipped;
    };
}
