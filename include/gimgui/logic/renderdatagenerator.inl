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
        RenderData& renderData = result[currentIndex];
        //set the current element pointer
        renderData.element = currentElement;

        //generate positions
        const Vec2& position = absolutePositions.getAbsoluteOf(*currentElement);
        const Vec2& size = currentElement->getAttribute<Vec2>("size");
        generateQuadPositions(position, size, renderData.positions);

        //generate colors
        const Color& color = currentElement->getAttribute<Color>("color");
        generateQuadColors(color, renderData.colors);

        //generate texcoords if the element has an image
        if(currentElement->hasAttribute<int32_t>("image_id"))
        {
            int32_t imageId = currentElement->getAttribute<int32_t>("image_id");
            GIM_ASSERT(currentElement->hasAttribute<Rectangle<Vec2>>("image_coords"), "currentElement has an image id registered but lacks 'image_coords'");
            GIM_ASSERT(mImageSizes.count(imageId) != 0, "image_id " + std::to_string(imageId) + " given to an currentElement but that id has not been registered in the RenderDataGenerator");

    const Rectangle<Vec2>& imageCoords = currentElement->getAttribute<Rectangle<Vec2>>("image_coords");

            const Vec2& imageSize = mImageSizes.at(imageId);

            renderData.imageId = imageId;

            std::array<float, 2> texCoordsStart;
            texCoordsStart[0] = (float)imageCoords.start.x / imageSize.x;
            texCoordsStart[1] = (float)imageCoords.start.y / imageSize.y;
            std::array<float, 2> texCoordsSize;
            texCoordsSize[0] = (float)imageCoords.size.x / imageSize.x;
            texCoordsSize[1] = (float)imageCoords.size.y / imageSize.y;

            generateQuadTexCoords(texCoordsStart, texCoordsSize, renderData.texCoords);
        }

        currentIndex++;
    }

    return result;
}

template <typename Vec2, typename Color>
void RenderDataGenerator<Vec2, Color>::registerImageInfo(int32_t imageId, const Vec2& imageSize)
{
    GIM_ASSERT(mImageSizes.count(imageId) == 0, "trying to add an image of id '" + std::to_string(imageId) + "' when such an image is already added");
    GIM_ASSERT(imageSize.x > 0 && imageSize.y > 0, "trying to add an image of size (" + std::to_string(imageSize.x) + "," + std::to_string(imageSize.y) + "). Both components must be above zero");

    mImageSizes[imageId] = imageSize;
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
void RenderDataGenerator<Vec2, Color>::generateQuadTexCoords(const std::array<float, 2>& texCoordStart, const std::array<float, 2>& texCoordSize, std::vector<float>& outTexCoords)
{
    std::vector<float> texCoordList(
    {
        texCoordStart[0]                     , texCoordStart[1]                  ,
        texCoordStart[0]                     , texCoordStart[1] + texCoordSize[1],
        texCoordStart[0] + texCoordSize[0]   , texCoordStart[1] + texCoordSize[1],
        texCoordStart[0] + texCoordSize[0]   , texCoordStart[1] + texCoordSize[1],
        texCoordStart[0] + texCoordSize[0]   , texCoordStart[1]                  ,
        texCoordStart[0]                     , texCoordStart[1]                     
    });

    outTexCoords.insert(outTexCoords.end(), texCoordList.begin(), texCoordList.end());
}
