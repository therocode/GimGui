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

        currentIndex++;
    }

    return result;
}
