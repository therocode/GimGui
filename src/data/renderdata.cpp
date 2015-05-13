#include <gimgui/data/renderdata.hpp>

namespace gim
{
    RenderData::RenderData(const RenderData& other):
        element(other.element),
        positions(other.positions),
        colors(other.colors),
        texCoords(other.texCoords),
        imageId(other.imageId),
        textPositions(other.textPositions),
        textColors(other.textColors),
        textTexCoords(other.textTexCoords),
        textImageId(other.textImageId)
    {
        if(other.clipRectangle)
            clipRectangle = std::unique_ptr<ClipRect>(new ClipRect(*other.clipRectangle));
    }

    RenderData& RenderData::operator=(const RenderData& other)
    {
        element = other.element;
        positions = other.positions;
        colors = other.colors;
        texCoords = other.texCoords;
        imageId = other.imageId;
        textPositions = other.textPositions;
        textColors = other.textColors;
        textTexCoords = other.textTexCoords;
        textImageId = other.textImageId;

        if(other.clipRectangle)
            clipRectangle = std::unique_ptr<ClipRect>(new ClipRect(*other.clipRectangle));

        return *this;
    }
}
