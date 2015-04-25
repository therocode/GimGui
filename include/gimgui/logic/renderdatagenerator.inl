template <typename Vec2, typename Color>
RenderDataGenerator<Vec2, Color>::RenderDataGenerator():
    mNextImageId(0),
    mNextFontId(0)
{
}

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
    const Vec2 position = absoluteMap.getAbsoluteOf(element);
    const Vec2 size = element.getAttribute<Vec2>("size");

    //generate colors, default white
    const Color& color = getOrFallback<Color>(element, "color", Color{255, 255, 255, 255});

    //generate texcoords if the element has an image
    const uint32_t* imageIdPtr = element.findAttribute<uint32_t>("image_id");
    //generate text quads if it has text
    const std::string* textPtr = element.findAttribute<std::string>("text");
    if(imageIdPtr != nullptr)
    {
        FloatVec2 mainQuadPosition{(float)position.x, (float)position.y};
        FloatVec2 mainQuadSize{(float)size.x, (float)size.y};
        GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("image_coords"), "currentElement has an image id registered but lacks 'image_coords'");
        GIM_ASSERT(mImageSizes.count(*imageIdPtr) != 0, "image_id " + std::to_string(*imageIdPtr) + " given to an currentElement but that id has not been registered in the RenderDataGenerator");

        uint32_t imageId = *imageIdPtr;
        StretchMode stretchMode = getOrFallback<StretchMode>(element, "stretch_mode", StretchMode::STRETCHED);
        const Rectangle<Vec2>& imageCoords = element.getAttribute<Rectangle<Vec2>>("image_coords");
        const Vec2& imageSizeInt = mImageSizes.at(imageId);
        const FloatVec2 imageSize{(float)imageSizeInt.x, (float)imageSizeInt.y};
        BorderMode borderMode = getOrFallback<BorderMode>(element, "border_mode", BorderMode::NONE);


        //adjust position and size based on borders
        if(borderMode == BorderMode::TOP_BOTTOM || borderMode == BorderMode::FULL)
        {//generate top and bottom borders
            GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("border_coords_t"), "currentElement set to have borders at top/bottom or full but lacks top border image coords");
            GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("border_coords_b"), "currentElement set to have borders at top/bottom or full but lacks bottom border image coords");

            const Rectangle<Vec2>& topImageCoords = element.getAttribute<Rectangle<Vec2>>("border_coords_t");
            const Rectangle<Vec2>& bottomImageCoords = element.getAttribute<Rectangle<Vec2>>("border_coords_b");

            mainQuadPosition.y += topImageCoords.size.y;
            mainQuadSize.y -= bottomImageCoords.size.y * 2;
        }

        if(borderMode == BorderMode::LEFT_RIGHT || borderMode == BorderMode::FULL)
        {//generate left and right borders
            GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("border_coords_l"), "currentElement set to have borders at left/right or full but lacks left border image coords");
            GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("border_coords_r"), "currentElement set to have borders at left/right or full but lacks right border image coords");

            const Rectangle<Vec2>& leftImageCoords = element.getAttribute<Rectangle<Vec2>>("border_coords_l");
            const Rectangle<Vec2>& rightImageCoords = element.getAttribute<Rectangle<Vec2>>("border_coords_r");

            mainQuadPosition.x += leftImageCoords.size.x;
            mainQuadSize.x -= rightImageCoords.size.x * 2;
        }

        //find out the amount of tiles it would have based on image size and size. Then based on stretch mode, set x, y or both, to 1. Then calculate underneath stuff and loop for all tiles
        Vec2 tileAmount({(int32_t)mainQuadSize.x / imageCoords.size.x + 1, (int32_t)mainQuadSize.y / imageCoords.size.y + 1});

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
                FloatVec2 quadPosition;
                quadPosition.x = mainQuadPosition.x + x * imageCoords.size.x;
                quadPosition.y = mainQuadPosition.y + y * imageCoords.size.y;

                FloatVec2 quadSize;

                bool lastX = x == tileAmount.x - 1;
                bool lastY = y == tileAmount.y - 1;

                quadSize.x = (!lastX) ? (imageCoords.size.x) : ((int32_t)mainQuadSize.x % imageCoords.size.x);
                quadSize.y = (!lastY) ? (imageCoords.size.y) : ((int32_t)mainQuadSize.y % imageCoords.size.y);

                FloatVec2 texCoordsStart;
                texCoordsStart.x = (float)imageCoords.start.x / imageSize.x;
                texCoordsStart.y = (float)imageCoords.start.y / imageSize.y;
                FloatVec2 texCoordsSize;
                texCoordsSize.x = (float)imageCoords.size.x / imageSize.x;
                texCoordsSize.y = (float)imageCoords.size.y / imageSize.y;

                if(stretchMode == StretchMode::STRETCHED)
                {
                    quadSize.x = mainQuadSize.x;
                    quadSize.y = mainQuadSize.y;
                }
                else if(stretchMode == StretchMode::V_TILED)
                {
                    quadSize.x = mainQuadSize.x;
                    texCoordsSize.y = lastY ? (quadSize.y / imageSize.y) : (texCoordsSize.y);
                }
                else if(stretchMode == StretchMode::H_TILED)
                {
                    texCoordsSize.x = lastX ? (quadSize.x / imageSize.x) : (texCoordsSize.x);
                    quadSize.y = mainQuadSize.y;
                }
                else if(stretchMode == StretchMode::TILED)
                {
                    texCoordsSize.x = lastX ? (quadSize.x / imageSize.x) : (texCoordsSize.x);
                    texCoordsSize.y = lastY ? (quadSize.y / imageSize.y) : (texCoordsSize.y);
                }

                generateQuadWithImage(quadPosition, quadSize, color, texCoordsStart, texCoordsSize, renderData.positions, renderData.colors, renderData.texCoords);
            }
        }
        generateBorders(element, mainQuadPosition, mainQuadSize, color, imageSize, renderData.positions, renderData.colors, renderData.texCoords);

        renderData.imageId = imageId;
    }

    if(textPtr != nullptr)
    {
        //get needed attributes
        const std::string utf8string = *textPtr;
        const gim::ref<Font>* fontPtr = element.findAttribute<gim::ref<Font>>("font");
        GIM_ASSERT(fontPtr != nullptr, "cannot give an element text without also giving it a font");
        Font& font = *fontPtr;
        GIM_ASSERT(mFontCacheIds.count(font.name()) != 0, "no texture registered for the given font '" + font.name() + "'");
        uint32_t fontCacheId = mFontCacheIds.at(font.name());

        const uint32_t* textSizeUPtr = element.findAttribute<uint32_t>("text_size");
        const int32_t* textSizePtr = element.findAttribute<int32_t>("text_size");
        GIM_ASSERT(textSizeUPtr != nullptr || textSizePtr != nullptr, "cannot give an element text without also giving it a text_size");
        uint32_t textSize = textSizeUPtr ? *textSizeUPtr : *textSizePtr;

        //get optional attributes
        const Color& color = getOrFallback<Color>(element, "text_color", Color{255, 255, 255, 255});
        float textScale = getOrFallback<float>(element, "text_scale", 1.0f);
        float characterSpacing = getOrFallback<float>(element, "character_spacing", 0.0f);
        float lineSpacing = getOrFallback<float>(element, "line_spacing", 0.0f);
        int32_t tabWidth = getOrFallback<int32_t>(element, "tab_width", 4);
        TextStyle style = getOrFallback<TextStyle>(element, "text_style", TextStyle::NORMAL);
        bool bold = style & TextStyle::BOLD;

        //render text
        gim::Utf8Decoder utf8Decoder;
        std::vector<uint32_t> codePoints = utf8Decoder.decode(utf8string);

        FontCacheEntry& fontCache = *mFontCache.at(fontCacheId);
        MetricsMap* metricsMap = &fontCache.metrics;
        FontTextureCache* textureCache = &fontCache.textureCoordinates; 

        Font* currentFont = &font;

        if(bold)
        {
            const gim::ref<Font>* boldFontPtr = element.findAttribute<gim::ref<Font>>("bold_font");
            GIM_ASSERT(boldFontPtr != nullptr, "cannot use TextStyle::BOLD with element lacking bold_font");
            currentFont = &boldFontPtr->get();
            GIM_ASSERT(mFontCacheIds.count(currentFont->name()) != 0, "no texture registered for the given bold_font '" + currentFont->name() + "'");
            uint32_t boldFontCacheId = mFontCacheIds.at(currentFont->name());

            FontCacheEntry& boldFontCache = *mFontCache.at(boldFontCacheId);
            metricsMap = &boldFontCache.metrics;
            textureCache = &boldFontCache.textureCoordinates;

            fontCacheId = boldFontCacheId;
        }

        renderData.textImageId = fontCache.textureId;

        float x = position.x;
        float y = position.y + textSize * textScale;
        float hspace = (getHSpace(*currentFont, textSize) + characterSpacing) * textScale;
        currentFont->resize(textSize);
        float vspace = (currentFont->lineSpacing() + lineSpacing) * textScale;
        uint32_t previousCodePoint = 0;

        TextureCoordinates texCoords;
        for(uint32_t codePoint : codePoints)
        {
            //special characters
            if(codePoint == ' ')
            {
                x += hspace;
                continue;
            }
            else if(codePoint == '\t')
            {
                x += hspace * tabWidth;
                continue;
            }
            else if(codePoint == '\n')
            {
                x = position.x;
                y += vspace;
                continue;
            }

            auto texCoordsPtr = textureCache->glyphCoords(codePoint, textSize, fontCacheId);

            Glyph::Metrics metrics;
            if(texCoordsPtr == nullptr)
            {
                auto glyphPtr = currentFont->generateGlyph(codePoint);

                if(glyphPtr)
                {
                    metrics = glyphPtr->metrics;
                    texCoords = textureCache->add(*glyphPtr, fontCacheId);
                    metricsMap->emplace(CodePointSizeId({codePoint, textSize, fontCacheId}), metrics);
                }
                else
                {//codepoint doesn't exist - do nothing
                    continue;
                }
            }
            else
            {
                GIM_ASSERT(metricsMap->count(CodePointSizeId({codePoint, textSize, fontCacheId})) != 0, "glyph existed in texture but not in metrics map. this is a bug.");
                texCoords = *texCoordsPtr;
                metrics = metricsMap->at(CodePointSizeId({codePoint, textSize, fontCacheId}));
            }
            
            x += currentFont->kerning(previousCodePoint, codePoint) * textScale;
            previousCodePoint = codePoint;

            float left = metrics.left * textScale;
            float top = metrics.top * textScale;
            float width = metrics.width * textScale;
            float height = metrics.height * textScale;

            //make quad
            generateQuadWithImage(FloatVec2({x + left, y + top}),
                                  FloatVec2({width, height}),
                                  color, 
                                  {texCoords.xStart, texCoords.yStart}, 
                                  {texCoords.xEnd - texCoords.xStart, texCoords.yEnd - texCoords.yStart},
                                   renderData.textPositions,
                                   renderData.textColors,
                                   renderData.textTexCoords,
                                   texCoords.flipped);

            x += (metrics.advance + characterSpacing) * textScale;
        }
    }

    return renderData;
}

template <typename Vec2, typename Color>
uint32_t RenderDataGenerator<Vec2, Color>::registerImageInfo(const Vec2& imageSize)
{
    GIM_ASSERT(imageSize.x > 0 && imageSize.y > 0, "trying to add an image of size (" + std::to_string(imageSize.x) + "," + std::to_string(imageSize.y) + "). Both components must be above zero");

    uint32_t newId = mNextImageId++;
    mImageSizes[newId] = imageSize;
    return newId;
}

template <typename Vec2, typename Color>
template <typename TextureAdaptor>
uint32_t RenderDataGenerator<Vec2, Color>::registerFontStorage(const std::vector<std::reference_wrapper<const Font>>& fonts, const TextureAdaptor& textureAdaptor)
{
    uint32_t newImageId = mNextImageId++;   

    std::shared_ptr<FontCacheEntry> textureCache = std::make_shared<FontCacheEntry>(FontCacheEntry{FontTextureCache(textureAdaptor), newImageId});
    for(auto& font : fonts)
    {
        uint32_t newFontId = mNextFontId++;   
        GIM_ASSERT(mFontCacheIds.count(font.get().name()) == 0, "trying to register font '" + font.get().name() + "' but it has already been registered");
        mFontCacheIds.emplace(font.get().name(), newFontId);
        mFontCache.emplace(newFontId, textureCache);
    }

    return newImageId;
}

template <typename Vec2, typename Color>
void RenderDataGenerator<Vec2, Color>::generateQuadWithoutImage(const FloatVec2& position, const FloatVec2& size, const Color& color, std::vector<float>& outPositions, std::vector<float>& outColors)
{
    generateQuadPositions(position, size, outPositions);
    generateQuadColors(color, outColors);
}

template <typename Vec2, typename Color>
void RenderDataGenerator<Vec2, Color>::generateQuadWithImage(const FloatVec2& position, const FloatVec2& size, const Color& color, const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outPositions, std::vector<float>& outColors, std::vector<float>& outTexCoords, bool flipTexCoords)
{
    generateQuadWithoutImage(position, size, color, outPositions, outColors);
    generateQuadTexCoords(texCoordStart, texCoordSize, outTexCoords, flipTexCoords);
}

template <typename Vec2, typename Color>
void RenderDataGenerator<Vec2, Color>::generateQuadPositions(const FloatVec2& position, const FloatVec2& size, std::vector<float>& outPositions)
{
    std::vector<float> triangle(
    {
        position.x         ,  position.y         , 0.0f,
        position.x         ,  position.y + size.y, 0.0f,
        position.x + size.x,  position.y + size.y, 0.0f,
        position.x + size.x,  position.y + size.y, 0.0f,
        position.x + size.x,  position.y         , 0.0f,
        position.x         ,  position.y         , 0.0f
    });

    outPositions.insert(outPositions.end(), triangle.begin(), triangle.end());
}

template <typename Vec2, typename Color>
void RenderDataGenerator<Vec2, Color>::generateQuadColors(const Color& color, std::vector<float>& outColors)
{
    float r = color.r / 255.0f;
    float g = color.g / 255.0f;
    float b = color.b / 255.0f;
    float a = color.a / 255.0f;

    std::vector<float> colorList(
    {
        r, g, b, a, 
        r, g, b, a, 
        r, g, b, a, 
        r, g, b, a, 
        r, g, b, a, 
        r, g, b, a
    });

    outColors.insert(outColors.end(), colorList.begin(), colorList.end());
}

template <typename Vec2, typename Color>
void RenderDataGenerator<Vec2, Color>::generateQuadTexCoords(const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outTexCoords, bool flipTexCoords)
{
    if(!flipTexCoords)
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
    else
    {
        std::vector<float> texCoordList(
                {
                texCoordStart.x                 , texCoordStart.y + texCoordSize.y,
                texCoordStart.x + texCoordSize.x, texCoordStart.y + texCoordSize.y,
                texCoordStart.x + texCoordSize.x, texCoordStart.y                 ,
                texCoordStart.x + texCoordSize.x, texCoordStart.y                 ,
                texCoordStart.x                 , texCoordStart.y                 ,
                texCoordStart.x                 , texCoordStart.y + texCoordSize.y,
                });

        outTexCoords.insert(outTexCoords.end(), texCoordList.begin(), texCoordList.end());
    }
}

template <typename Vec2, typename Color>
void RenderDataGenerator<Vec2, Color>::generateBorders(const Element& element, const FloatVec2& position, const FloatVec2& size, const Color& color, const FloatVec2& imageSize, std::vector<float>& outPositions, std::vector<float>& outColors, std::vector<float>& outTexCoords)
{
    const BorderMode* borderModePtr = element.findAttribute<BorderMode>("border_mode");
    BorderMode borderMode = borderModePtr ? *borderModePtr : BorderMode::NONE;

    if(borderMode == BorderMode::TOP_BOTTOM || borderMode == BorderMode::FULL)
    {//generate top and bottom borders
        GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("border_coords_t"), "currentElement set to have borders at top/bottom or full but lacks top border image coords");
        GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("border_coords_b"), "currentElement set to have borders at top/bottom or full but lacks bottom border image coords");

        const Rectangle<Vec2>& topImageCoords = element.getAttribute<Rectangle<Vec2>>("border_coords_t");
        const Rectangle<Vec2>& bottomImageCoords = element.getAttribute<Rectangle<Vec2>>("border_coords_b");

        //top quad
        generateQuadWithImage(
                FloatVec2({position.x, position.y - topImageCoords.size.y}),
                FloatVec2({size.x, (float)topImageCoords.size.y}),
                color,
                FloatVec2({(float)topImageCoords.start.x / imageSize.x, (float)topImageCoords.start.y / imageSize.y}),
                FloatVec2({(float)topImageCoords.size.x  / imageSize.x, (float)topImageCoords.size.y  / imageSize.y}),
                outPositions, outColors, outTexCoords);
        
        //bottom quad
        generateQuadWithImage(
                FloatVec2({position.x, position.y + size.y}),
                FloatVec2({size.x, (float)bottomImageCoords.size.y}),
                color,
                FloatVec2({(float)bottomImageCoords.start.x / imageSize.x, (float)bottomImageCoords.start.y / imageSize.y}),
                FloatVec2({(float)bottomImageCoords.size.x  / imageSize.x, (float)bottomImageCoords.size.y  / imageSize.y}),
                outPositions, outColors, outTexCoords);
    }

    if(borderMode == BorderMode::LEFT_RIGHT || borderMode == BorderMode::FULL)
    {//generate left and right borders
        GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("border_coords_l"), "currentElement set to have borders at left/right or full but lacks left border image coords");
        GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("border_coords_r"), "currentElement set to have borders at left/right or full but lacks right border image coords");

        const Rectangle<Vec2>& leftImageCoords = element.getAttribute<Rectangle<Vec2>>("border_coords_l");
        const Rectangle<Vec2>& rightImageCoords = element.getAttribute<Rectangle<Vec2>>("border_coords_r");

        //left quad
        generateQuadWithImage(
                FloatVec2({position.x - leftImageCoords.size.x, position.y}),
                FloatVec2({(float)leftImageCoords.size.x, size.y}),
                color,
                FloatVec2({(float)leftImageCoords.start.x / imageSize.x, (float)leftImageCoords.start.y / imageSize.y}),
                FloatVec2({(float)leftImageCoords.size.x  / imageSize.x, (float)leftImageCoords.size.y  / imageSize.y}),
                outPositions, outColors, outTexCoords);
        
        //right quad
        generateQuadWithImage(
                FloatVec2({position.x + size.x, position.y}),
                FloatVec2({(float)rightImageCoords.size.x, size.y}),
                color,
                FloatVec2({(float)rightImageCoords.start.x / imageSize.x, (float)rightImageCoords.start.y / imageSize.y}),
                FloatVec2({(float)rightImageCoords.size.x  / imageSize.x, (float)rightImageCoords.size.y  / imageSize.y}),
                outPositions, outColors, outTexCoords);
    }

    if(borderMode == BorderMode::FULL)
    {//generate corner borders
        GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("border_coords_tl"), "currentElement set to have full borders but lacks top-left border image coords");
        GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("border_coords_tr"), "currentElement set to have full borders but lacks top-right border image coords");
        GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("border_coords_bl"), "currentElement set to have full borders but lacks bottom-left border image coords");
        GIM_ASSERT(element.hasAttribute<Rectangle<Vec2>>("border_coords_br"), "currentElement set to have full borders but lacks bottom-right border image coords");

        const Rectangle<Vec2>& topLeftImageCoords = element.getAttribute<Rectangle<Vec2>>("border_coords_tl");
        const Rectangle<Vec2>& topRightImageCoords = element.getAttribute<Rectangle<Vec2>>("border_coords_tr");
        const Rectangle<Vec2>& bottomLeftImageCoords = element.getAttribute<Rectangle<Vec2>>("border_coords_bl");
        const Rectangle<Vec2>& bottomRightImageCoords = element.getAttribute<Rectangle<Vec2>>("border_coords_br");

        //topLeft quad
        generateQuadWithImage(
                FloatVec2({position.x - topLeftImageCoords.size.x, position.y - topLeftImageCoords.size.y}),
                {(float)topLeftImageCoords.size.x, (float)topLeftImageCoords.size.y},
                color,
                FloatVec2({(float)topLeftImageCoords.start.x / imageSize.x, (float)topLeftImageCoords.start.y / imageSize.y}),
                FloatVec2({(float)topLeftImageCoords.size.x  / imageSize.x, (float)topLeftImageCoords.size.y  / imageSize.y}),
                outPositions, outColors, outTexCoords);
        
        //topRight quad
        generateQuadWithImage(
                FloatVec2({position.x + size.x, position.y - topRightImageCoords.size.y}),
                {(float)topRightImageCoords.size.x, (float)topRightImageCoords.size.y},
                color,
                FloatVec2({(float)topRightImageCoords.start.x / imageSize.x, (float)topRightImageCoords.start.y / imageSize.y}),
                FloatVec2({(float)topRightImageCoords.size.x  / imageSize.x, (float)topRightImageCoords.size.y  / imageSize.y}),
                outPositions, outColors, outTexCoords);

        //bottomLeft quad
        generateQuadWithImage(
                FloatVec2({position.x - bottomLeftImageCoords.size.y, position.y + size.y}),
                {(float)bottomLeftImageCoords.size.x, (float)bottomLeftImageCoords.size.y},
                color,
                FloatVec2({(float)bottomLeftImageCoords.start.x / imageSize.x, (float)bottomLeftImageCoords.start.y / imageSize.y}),
                FloatVec2({(float)bottomLeftImageCoords.size.x  / imageSize.x, (float)bottomLeftImageCoords.size.y  / imageSize.y}),
                outPositions, outColors, outTexCoords);
        
        //bottomRight quad
        generateQuadWithImage(
                FloatVec2({position.x + size.x, position.y + size.y}),
                {(float)bottomRightImageCoords.size.x, (float)bottomRightImageCoords.size.y},
                color,
                FloatVec2({(float)bottomRightImageCoords.start.x / imageSize.x, (float)bottomRightImageCoords.start.y / imageSize.y}),
                FloatVec2({(float)bottomRightImageCoords.size.x  / imageSize.x, (float)bottomRightImageCoords.size.y  / imageSize.y}),
                outPositions, outColors, outTexCoords);
    }
}

template <typename Vec2, typename Color>
float RenderDataGenerator<Vec2, Color>::getHSpace(const Font& font, uint32_t size)
{
    uint32_t whitespace = ' ';
    uint32_t fontId = mFontCacheIds.at(font.name());
    auto& fontCache = *mFontCache.at(fontId);

    auto metricsIter = fontCache.metrics.find(CodePointSizeId({whitespace, size, fontId}));
    
    if(metricsIter != fontCache.metrics.end())
    {
        return metricsIter->second.advance;
    }
    else
    {
        auto glyphPtr = font.generateGlyph(whitespace);

        GIM_ASSERT(glyphPtr != nullptr, "font didn't contain codepoint for ' ' (whitespace)");

        fontCache.textureCoordinates.add(*glyphPtr, fontId);
        fontCache.metrics.emplace(CodePointSizeId({whitespace, size, fontId}), glyphPtr->metrics);

        return glyphPtr->metrics.advance;
    }
}
