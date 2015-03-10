#pragma once

template <typename Vec2, typename Color>
std::vector<RenderData> RenderDataGenerator<Vec2, Color>::generate(const gim::Element& element)
{
    gim::AllConstPropagator all(element);
    std::vector<RenderData> result(all.size());

    const gim::Element* currentElement;

    gim::AbsoluteMap<Vec2> absolutePositions("position");

    size_t currentIndex = 0;
    while((currentElement = all.next()))
    {
        result[currentIndex] = generateElementData(*currentElement, absolutePositions);

        currentIndex++;
    }

    return result;
}

template <typename Vec2, typename Color>
RenderData RenderDataGenerator<Vec2, Color>::generateElementData(const Element& element, gim::AbsoluteMap<Vec2>& absoluteMap)
{
    RenderData renderData;

    //set the current element pointer
    renderData.element = &element;

    //generate positions
    const Vec2& position = absoluteMap.getAbsoluteOf(element);
    const Vec2& size = element.getAttribute<Vec2>("size");

    //generate colors, default white
    const Color* colorPtr = element.findAttribute<Color>("color");
    const Color& color = colorPtr ? *colorPtr : Color({255, 255, 255, 255});

    //generate texcoords if the element has an image
    const int32_t* imageIdPtr = element.findAttribute<int32_t>("image_id");
    if(imageIdPtr != nullptr)
    {
        GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("image_coords"), "currentElement has an image id registered but lacks 'image_coords'");
        GIM_ASSERT(mImageSizes.count(*imageIdPtr) != 0, "image_id " + std::to_string(*imageIdPtr) + " given to an currentElement but that id has not been registered in the RenderDataGenerator");

        int32_t imageId = *imageIdPtr;
        const StretchMode* stretchModePtr = element.findAttribute<StretchMode>("stretch_mode");
        StretchMode stretchMode = stretchModePtr ? *stretchModePtr : StretchMode::STRETCHED;
        const BorderMode* borderModePtr = element.findAttribute<BorderMode>("border_mode");
        BorderMode borderMode = borderModePtr ? *borderModePtr : BorderMode::NONE;
        const Rectangle<Vec2>& imageCoords = element.getAttribute<Rectangle<Vec2>>("image_coords");
        const Vec2& imageSize = mImageSizes.at(imageId);

        //find out the amount of tiles it would have based on image size and size. Then based on stretch mode, set x, y or both, to 1. Then calculate underneath stuff and loop for all tiles
        Vec2 tileAmount({size.x / imageCoords.size.x + 1, size.y / imageCoords.size.y + 1});

        if(stretchMode == StretchMode::STRETCHED)
        {
            tileAmount.x = 1;
            tileAmount.y = 1;
        }
        else if(stretchMode == StretchMode::V_TILED)
        {
            tileAmount.x = 1;
        }
        else if(stretchMode == StretchMode::H_TILED)
        {
            tileAmount.y = 1;
        }

        for(int32_t y = 0; y < tileAmount.y; y++)
        {
            for(int32_t x = 0; x < tileAmount.x; x++)
            {
                Vec2 quadPosition;
                quadPosition.x = position.x + x * imageCoords.size.x;
                quadPosition.y = position.y + y * imageCoords.size.y;

                Vec2 quadSize;

                quadSize.x = (x < tileAmount.x - 1) ? (imageCoords.size.x) : (size.x % imageCoords.size.x);
                quadSize.y = (y < tileAmount.y - 1) ? (imageCoords.size.y) : (size.y % imageCoords.size.y);

                if(stretchMode == StretchMode::STRETCHED)
                {
                    quadSize.x = size.x;
                    quadSize.y = size.y;
                }
                else if(stretchMode == StretchMode::V_TILED)
                {
                    quadSize.x = size.x;
                }
                else if(stretchMode == StretchMode::H_TILED)
                {
                    quadSize.y = size.y;
                }

                FloatVec2 texCoordsStart;
                texCoordsStart.x = (float)imageCoords.start.x / imageSize.x;
                texCoordsStart.y = (float)imageCoords.start.y / imageSize.y;
                FloatVec2 texCoordsSize;
                texCoordsSize.x = (float)imageCoords.size.x / imageSize.x;
                texCoordsSize.y = (float)imageCoords.size.y / imageSize.y;

                generateQuadWithImage(quadPosition, quadSize, color, texCoordsStart, texCoordsSize, renderData.positions, renderData.colors, renderData.texCoords);
            }
        }

        renderData.imageId = imageId;
    }
    else
    {
        generateQuadWithoutImage(position, size, color, renderData.positions, renderData.colors);
    }

    return renderData;
}

template <typename Vec2, typename Color>
void RenderDataGenerator<Vec2, Color>::registerImageInfo(int32_t imageId, const Vec2& imageSize)
{
    GIM_ASSERT(mImageSizes.count(imageId) == 0, "trying to add an image of id '" + std::to_string(imageId) + "' when such an image is already added");
    GIM_ASSERT(imageSize.x > 0 && imageSize.y > 0, "trying to add an image of size (" + std::to_string(imageSize.x) + "," + std::to_string(imageSize.y) + "). Both components must be above zero");

    mImageSizes[imageId] = imageSize;
}

template <typename Vec2, typename Color>
void RenderDataGenerator<Vec2, Color>::generateQuadWithoutImage(const Vec2& position, const Vec2& size, const Color& color, std::vector<float>& outPositions, std::vector<float>& outColors)
{
    generateQuadPositions(position, size, outPositions);
    generateQuadColors(color, outColors);
}

template <typename Vec2, typename Color>
void RenderDataGenerator<Vec2, Color>::generateQuadWithImage(const Vec2& position, const Vec2& size, const Color& color, const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outPositions, std::vector<float>& outColors, std::vector<float>& outTexCoords)
{
    generateQuadWithoutImage(position, size, color, outPositions, outColors);
    generateQuadTexCoords(texCoordStart, texCoordSize, outTexCoords);
}

template <typename Vec2, typename Color>
void RenderDataGenerator<Vec2, Color>::generateQuadPositions(const Vec2& position, const Vec2& size, std::vector<float>& outPositions)
{
    std::vector<float> triangle(
    {
        (float)position.x         ,  (float)position.y         , 0.0f,
        (float)position.x         ,  (float)position.y + size.y, 0.0f,
        (float)position.x + size.x,  (float)position.y + size.y, 0.0f,
        (float)position.x + size.x,  (float)position.y + size.y, 0.0f,
        (float)position.x + size.x,  (float)position.y         , 0.0f,
        (float)position.x         ,  (float)position.y         , 0.0f
    });

    outPositions.insert(outPositions.end(), triangle.begin(), triangle.end());
}

template <typename Vec2, typename Color>
void RenderDataGenerator<Vec2, Color>::generateQuadColors(const Color& color, std::vector<float>& outColors)
{
    std::vector<float> colorList(
    {
        color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 
        color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 
        color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
        color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
        color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
        color.r / 255.0f, color.g / 255.0f, color.b / 255.0f
    });

    outColors.insert(outColors.end(), colorList.begin(), colorList.end());
}

template <typename Vec2, typename Color>
void RenderDataGenerator<Vec2, Color>::generateQuadTexCoords(const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outTexCoords)
{
    std::vector<float> texCoordList(
    {
        texCoordStart.x                 , texCoordStart.y                 ,
        texCoordStart.x                 , texCoordStart.y + texCoordSize.y,
        texCoordStart.x + texCoordSize.x, texCoordStart.y + texCoordSize.y,
        texCoordStart.x + texCoordSize.x, texCoordStart.y + texCoordSize.y,
        texCoordStart.x + texCoordSize.x, texCoordStart.y                 ,
        texCoordStart.x                 , texCoordStart.y                     
    });

    outTexCoords.insert(outTexCoords.end(), texCoordList.begin(), texCoordList.end());
}
