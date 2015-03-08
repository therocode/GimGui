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
        auto& triangles = result[currentIndex].positions;
        auto& colors = result[currentIndex].colors;
        auto& texCoords = result[currentIndex].texCoords;

        const Vec2& position = absolutePositions.getAbsoluteOf(*currentElement);
        const Vec2& size = currentElement->getAttribute<Vec2>("size");
        std::vector<float> triangle(
        {
            (float)position.x         ,  (float)position.y         , 0.0f,
            (float)position.x         ,  (float)position.y + size.y, 0.0f,
            (float)position.x + size.x,  (float)position.y + size.y, 0.0f,
            (float)position.x + size.x,  (float)position.y + size.y, 0.0f,
            (float)position.x + size.x,  (float)position.y         , 0.0f,
            (float)position.x         ,  (float)position.y         , 0.0f
        });

        triangles.insert(triangles.end(), triangle.begin(), triangle.end());

        const Color& color = currentElement->getAttribute<Color>("color");

        std::vector<float> colorList(
        {
            color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 
            color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 
            color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
            color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
            color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
            color.r / 255.0f, color.g / 255.0f, color.b / 255.0f
        });

        colors.insert(colors.end(), colorList.begin(), colorList.end());

        if(currentElement->hasAttribute<int32_t>("image_id"))
        {
            int32_t imageId = currentElement->getAttribute<int32_t>("image_id");
            GIM_ASSERT(currentElement->hasAttribute<Rectangle<Vec2>>("image_coords"), "currentElement has an image id registered but lacks 'image_coords'");
            GIM_ASSERT(mImageSizes.count(imageId) != 0, "image_id " + std::to_string(imageId) + " given to an currentElement but that id has not been registered in the RenderDataGenerator");

            const Vec2& imageSize = mImageSizes.at(imageId);
            const Rectangle<Vec2>& imageCoords = currentElement->getAttribute<Rectangle<Vec2>>("image_coords");

            std::vector<float> texCoordList(
            {
                (float)(imageCoords.start.x                     ) / imageSize.x, (float)(imageCoords.start.y                     ) / imageSize.y,
                (float)(imageCoords.start.x                     ) / imageSize.x, (float)(imageCoords.start.y + imageCoords.size.y) / imageSize.y,
                (float)(imageCoords.start.x + imageCoords.size.x) / imageSize.x, (float)(imageCoords.start.y + imageCoords.size.y) / imageSize.y,
                (float)(imageCoords.start.x + imageCoords.size.x) / imageSize.x, (float)(imageCoords.start.y + imageCoords.size.y) / imageSize.y,
                (float)(imageCoords.start.x + imageCoords.size.x) / imageSize.x, (float)(imageCoords.start.y                     ) / imageSize.y,
                (float)(imageCoords.start.x                     ) / imageSize.x, (float)(imageCoords.start.y                     ) / imageSize.y,
            });

            texCoords.insert(texCoords.end(), texCoordList.begin(), texCoordList.end());

            result[currentIndex].imageId = imageId;
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
