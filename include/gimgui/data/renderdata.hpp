#pragma once
#include <vector>
#include <memory>

namespace gim
{
    class Element;

    template<typename Texture>
    struct RenderData
    {
        struct ClipRect
        {
            int32_t xStart;
            int32_t yStart;
            int32_t width;
            int32_t height;
        };

        RenderData() = default;
        RenderData(const RenderData& other);
        RenderData& operator=(const RenderData& other);
        RenderData(RenderData&& other) = default;
        RenderData& operator=(RenderData&& other) = default;

        const Element* element;
        std::vector<float> positions;
        std::vector<float> colors;
        std::vector<float> texCoords;
        const Texture* texture;

        std::vector<float> textPositions;
        std::vector<float> textColors;
        std::vector<float> textTexCoords;
        const Texture* textTexture;
        
        std::unique_ptr<ClipRect> clipRectangle;
    };

    template<typename Texture>
    RenderData<Texture>::RenderData(const RenderData<Texture>& other):
        element(other.element),
        positions(other.positions),
        colors(other.colors),
        texCoords(other.texCoords),
        texture(other.texture),
        textPositions(other.textPositions),
        textColors(other.textColors),
        textTexCoords(other.textTexCoords),
        textTexture(other.textTexture)
    {
        if(other.clipRectangle)
            clipRectangle = std::unique_ptr<ClipRect>(new ClipRect(*other.clipRectangle));
    }

    template<typename Texture>
    RenderData<Texture>& RenderData<Texture>::operator=(const RenderData& other)
    {
        element = other.element;
        positions = other.positions;
        colors = other.colors;
        texCoords = other.texCoords;
        texture= other.texture;
        textPositions = other.textPositions;
        textColors = other.textColors;
        textTexCoords = other.textTexCoords;
        textTexture= other.textTexture;

        if(other.clipRectangle)
            clipRectangle = std::unique_ptr<ClipRect>(new ClipRect(*other.clipRectangle));

        return *this;
    }
}
