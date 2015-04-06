#include <gimgui/data/texturecoordinates.hpp>

namespace gim
{
    bool TextureCoordinates::operator==(const TextureCoordinates& other)
    {
        return xStart == other.xStart &&
               yStart == other.yStart &&
               xEnd == other.xEnd &&
               yEnd == other.yEnd &&
               flipped == other.flipped;
    }
}
