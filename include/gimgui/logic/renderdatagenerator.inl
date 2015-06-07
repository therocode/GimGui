template <typename Vec2, typename Rectangle, typename Color>
RenderDataGenerator<Vec2, Rectangle, Color>::RenderDataGenerator():
    mNextTextureId(0),
    mNextFontId(0)
{
}

template <typename Vec2, typename Rectangle, typename Color>
std::vector<RenderData> RenderDataGenerator<Vec2, Rectangle, Color>::generate(const gim::Element& element)
{
    gim::AllConstPropagator all(element);
    std::vector<RenderData> result(all.size());

    const gim::Element* currentElement;

    gim::AbsoluteMap<typename Vec2::Native> absolutePositions("position");

    size_t currentIndex = 0;
    while((currentElement = all.next()))
    {
        result[currentIndex] = generateElementData(*currentElement, absolutePositions);

        currentIndex++;
    }

    return result;
}

template <typename Vec2, typename Rectangle, typename Color>
RenderData RenderDataGenerator<Vec2, Rectangle, Color>::generateElementData(const Element& element, gim::AbsoluteMap<typename Vec2::Native>& absoluteMap)
{
    RenderData renderData;

    //set the current element pointer
    renderData.element = &element;

    //generate positions
    const Vec2 position = absoluteMap.getAbsoluteOf(element);
    const Vec2 size = element.getAttribute<typename Vec2::Native>("size");
    const float zPosition = getOrFallback<float>(element, "z_position", 0.0f);

    //generate colors, default white
    const typename Color::Native* nativeColorPtr = element.findAttribute<typename Color::Native>("color");
    const Color color = nativeColorPtr ? *nativeColorPtr : Color(255, 255, 255, 255);

    //generate texcoords if the element has an image
    const uint32_t* imageIdPtr = element.findAttribute<uint32_t>("image_id");
    //generate text quads if it has text
    const std::string* textPtr = element.findAttribute<std::string>("text");
    if(imageIdPtr != nullptr)
    {
        FloatVec2 mainQuadPosition{(float)position.x(), (float)position.y()};
        FloatVec2 mainQuadSize{(float)size.x(), (float)size.y()};
        GIM_ASSERT(element.hasAttribute<typename Rectangle::Native>("image_coords"), "currentElement has an image id registered but lacks 'image_coords'");
        GIM_ASSERT(mTextureSizes.count(*imageIdPtr) != 0, "image_id " + std::to_string(*imageIdPtr) + " given to an currentElement but that id has not been registered in the RenderDataGenerator");

        uint32_t imageId = *imageIdPtr;
        StretchMode stretchMode = getOrFallback<StretchMode>(element, "stretch_mode", StretchMode::STRETCHED);
        const Rectangle& imageCoords = element.getAttribute<typename Rectangle::Native>("image_coords");
        const Vec2& imageSizeInt = mTextureSizes.at(imageId);
        const FloatVec2 imageSize{(float)imageSizeInt.x(), (float)imageSizeInt.y()};
        BorderMode borderMode = getOrFallback<BorderMode>(element, "border_mode", BorderMode::NONE);


        //adjust position and size based on borders
        if(borderMode == BorderMode::TOP_BOTTOM || borderMode == BorderMode::FULL)
        {//generate top and bottom borders
            GIM_ASSERT(element.hasAttribute<typename Rectangle::Native>("border_coords_t"), "currentElement set to have borders at top/bottom or full but lacks top border image coords");
            GIM_ASSERT(element.hasAttribute<typename Rectangle::Native>("border_coords_b"), "currentElement set to have borders at top/bottom or full but lacks bottom border image coords");

            const Rectangle& topImageCoords = element.getAttribute<typename Rectangle::Native>("border_coords_t");
            const Rectangle& bottomImageCoords = element.getAttribute<typename Rectangle::Native>("border_coords_b");

            mainQuadPosition.y += topImageCoords.size().y();
            mainQuadSize.y -= bottomImageCoords.size().y() * 2;
        }

        if(borderMode == BorderMode::LEFT_RIGHT || borderMode == BorderMode::FULL)
        {//generate left and right borders
            GIM_ASSERT(element.hasAttribute<typename Rectangle::Native>("border_coords_l"), "currentElement set to have borders at left/right or full but lacks left border image coords");
            GIM_ASSERT(element.hasAttribute<typename Rectangle::Native>("border_coords_r"), "currentElement set to have borders at left/right or full but lacks right border image coords");

            const Rectangle& leftImageCoords = element.getAttribute<typename Rectangle::Native>("border_coords_l");
            const Rectangle& rightImageCoords = element.getAttribute<typename Rectangle::Native>("border_coords_r");

            mainQuadPosition.x += leftImageCoords.size().x();
            mainQuadSize.x -= rightImageCoords.size().x() * 2;
        }

        //find out the amount of tiles it would have based on image size and size. Then based on stretch mode, set x, y or both, to 1. Then calculate underneath stuff and loop for all tiles
        Vec2 tileAmount((int32_t)mainQuadSize.x / imageCoords.size().x() + 1, (int32_t)mainQuadSize.y / imageCoords.size().y() + 1);

        if(stretchMode == StretchMode::STRETCHED)
        {
            tileAmount.x(1);
            tileAmount.y(1);
        }
        else if(stretchMode == StretchMode::V_TILED)
        {
            tileAmount.x(1);
        }
        else if(stretchMode == StretchMode::H_TILED)
        {
            tileAmount.y(1);
        }

        for(int32_t y = 0; y < tileAmount.y(); y++)
        {
            for(int32_t x = 0; x < tileAmount.x(); x++)
            {
                FloatVec2 quadPosition;
                quadPosition.x = mainQuadPosition.x + x * imageCoords.size().x();
                quadPosition.y = mainQuadPosition.y + y * imageCoords.size().y();

                FloatVec2 quadSize;

                bool lastX = x == tileAmount.x() - 1;
                bool lastY = y == tileAmount.y() - 1;

                quadSize.x = (!lastX) ? (imageCoords.size().x()) : ((int32_t)mainQuadSize.x % imageCoords.size().x());
                quadSize.y = (!lastY) ? (imageCoords.size().y()) : ((int32_t)mainQuadSize.y % imageCoords.size().y());

                FloatVec2 texCoordsStart;
                texCoordsStart.x = (float)imageCoords.start().x() / imageSize.x;
                texCoordsStart.y = (float)imageCoords.start().y() / imageSize.y;
                FloatVec2 texCoordsSize;
                texCoordsSize.x = (float)imageCoords.size().x() / imageSize.x;
                texCoordsSize.y = (float)imageCoords.size().y() / imageSize.y;

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

                generateQuadWithImage(quadPosition, quadSize, color, zPosition, texCoordsStart, texCoordsSize, renderData.positions, renderData.colors, renderData.texCoords);
            }
        }
        generateBorders(element, mainQuadPosition, mainQuadSize, color, zPosition, imageSize, renderData.positions, renderData.colors, renderData.texCoords);

        renderData.imageId = imageId;
    }

    if(textPtr != nullptr)
    {
        const std::string utf8string = *textPtr;

        //get needed attributes
        const uint32_t* textSizeUPtr = element.findAttribute<uint32_t>("text_size");
        const int32_t* textSizePtr = element.findAttribute<int32_t>("text_size");
        GIM_ASSERT(textSizeUPtr != nullptr || textSizePtr != nullptr, "cannot give an element text without also giving it a text_size");
        uint32_t textSize = textSizeUPtr ? *textSizeUPtr : *textSizePtr;
        const float textZPosition = getOrFallback<float>(element, "text_z_position", 0.0f);

        //get optional attributes
        const typename Color::Native* nativeColorPtr = element.findAttribute<typename Color::Native>("text_color");
        Color color = nativeColorPtr ? *nativeColorPtr : Color(255, 255, 255, 255);
        float textScale = getOrFallback<float>(element, "text_scale", 1.0f);
        float characterSpacing = getOrFallback<float>(element, "character_spacing", 0.0f);
        float lineSpacing = getOrFallback<float>(element, "line_spacing", 0.0f);
        int32_t tabWidth = getOrFallback<int32_t>(element, "tab_width", 4);
        TextStyle style = getOrFallback<TextStyle>(element, "text_style", TextStyle::NORMAL);
        const typename Rectangle::Native* nativeTextBordersPtr = element.findAttribute<typename Rectangle::Native>("text_borders");
        Rectangle textBorders = nativeTextBordersPtr ? *nativeTextBordersPtr :  Rectangle(Vec2(0, 0), Vec2(size));
        const WrapMode wrapMode = getOrFallback<WrapMode>(element, "line_wrap", WrapMode::WORDS);
        const TextAlign textAlign = getOrFallback<TextAlign>(element, "text_alignment", TextAlign::LEFT);
        bool clipText = getOrFallback<bool>(element, "text_clipping", true);

        MetricsMap* currentMetricsMap;
        internal::FontTextureCache* currentTextureCache; 
        Font* currentFont;
        uint32_t currentFontCacheId;
        uint32_t currentFontTextureId;

        auto loadFont = [this, &currentMetricsMap, &currentTextureCache, &currentFont, &currentFontCacheId, &currentFontTextureId] (const std::string& font)
        {
            GIM_ASSERT(mFontCacheIds.count(font) != 0, "The given font '" + font + "' is not registered");
            GIM_ASSERT(mFonts.count(font) != 0, "texture registered, but not existing. This is a bug in gimgui");
            currentFontCacheId = mFontCacheIds.at(font);
            FontCacheEntry& fontCache = *mFontCache.at(currentFontCacheId);
            currentMetricsMap = &fontCache.metrics;
            currentTextureCache = &fontCache.textureCoordinates; 
            currentFont = &mFonts.at(font).get();
            currentFontTextureId = fontCache.textureId;
        };

        if(!(style & TextStyle::BOLD) && !(style & TextStyle::ITALIC))
        {
            const std::string* fontNamePtr = element.findAttribute<std::string>("font");
            GIM_ASSERT(fontNamePtr != nullptr, "cannot use TextStyle::NORMAL without setting the 'font' attribute to a font");
            loadFont(*fontNamePtr);
        }
        else if((style & TextStyle::BOLD) && !(style & TextStyle::ITALIC))
        {
            const std::string* fontNamePtr = element.findAttribute<std::string>("bold_font");
            GIM_ASSERT(fontNamePtr != nullptr, "cannot use TextStyle::BOLD without setting the 'bold_font' attribute to a font");
            loadFont(*fontNamePtr);
        }
        else if(!(style & TextStyle::BOLD) && (style & TextStyle::ITALIC))
        {
            const std::string* fontNamePtr = element.findAttribute<std::string>("italic_font");
            GIM_ASSERT(fontNamePtr != nullptr, "cannot use TextStyle::ITALIC without setting the 'italic_font' attribute to a font");
            loadFont(*fontNamePtr);
        }
        else if((style & TextStyle::BOLD) && (style & TextStyle::ITALIC))
        {
            const std::string* fontNamePtr = element.findAttribute<std::string>("bold_italic_font");
            GIM_ASSERT(fontNamePtr != nullptr, "cannot use TextStyle::BOLD and TextStyle::ITALIC without setting the 'bold_italic_font' attribute to a font");
            loadFont(*fontNamePtr);
        }

        //render text
        gim::Utf8Decoder utf8Decoder;
        auto codePointsVector = utf8Decoder.decode(utf8string);
        std::deque<uint32_t> codePoints(codePointsVector.begin(), codePointsVector.end());

        renderData.textImageId = currentFontTextureId;

        const Vec2 textStart(position.x() + textBorders.start().x(), position.y() + textBorders.start().y());
        float x = textStart.x();
        float y = textStart.y() + textSize * textScale;
        float hspace = (getHSpace(*currentFont, textSize) + characterSpacing) * textScale;
        currentFont->resize(textSize);
        float vspace = (currentFont->lineSpacing() + lineSpacing) * textScale;
        uint32_t previousCodePoint = 0;

        if(clipText)
        {
            renderData.clipRectangle = std::unique_ptr<RenderData::ClipRect>(new RenderData::ClipRect());
            renderData.clipRectangle->xStart = textStart.x();
            renderData.clipRectangle->yStart = textStart.y();
            renderData.clipRectangle->width = textBorders.size().x();
            renderData.clipRectangle->height = textBorders.size().y();
        }

        TextureCoordinates texCoords;

        struct CharacterQuad
        {
            FloatVec2 start;
            FloatVec2 size;
            Color color;
            TextureCoordinates texCoords;
            uint32_t codePoint;
        };

        std::vector<CharacterQuad> currentRow;

        auto flushRow = [this, &renderData, &textAlign, &textBorders, &textStart, &textZPosition] (std::vector<CharacterQuad>& quads, bool lineFull)
        {
            if(!quads.empty())
            {
                float rowOffset = 0.0f;

                if(textAlign == TextAlign::RIGHT)
                {
                    rowOffset = textBorders.size().x() - quads.back().start.x - quads.back().size.x + textStart.x();
                }
                else if(textAlign == TextAlign::CENTER)
                {
                    rowOffset = std::floor((textBorders.size().x() - quads.back().start.x - quads.back().size.x + textStart.x()) / 2.0f);
                }

                for(const auto& quad : quads)
                {
                    FloatVec2 start({quad.start.x + rowOffset, quad.start.y});
                    generateQuadWithImage(start, quad.size, quad.color, textZPosition, {quad.texCoords.xStart, quad.texCoords.yStart}, {quad.texCoords.xEnd - quad.texCoords.xStart, quad.texCoords.yEnd - quad.texCoords.yStart}, renderData.textPositions, renderData.textColors, renderData.textTexCoords, quad.texCoords.flipped);
                }
                quads.clear();
            }
        };

        auto newLine = [&x, &y, &textStart, &vspace, flushRow] (std::vector<CharacterQuad>& quads, bool lineFull)
        {
            flushRow(quads, lineFull);
            x = textStart.x();
            y += vspace;
        };

        size_t currentWordStart = 0;
        int32_t wordsSinceLineStart = 0;
        for(uint32_t codePointIndex = 0; codePointIndex < codePoints.size(); codePointIndex++)
        {
            uint32_t codePoint = codePoints[codePointIndex];
            //special characters
            if(codePoint == ' ')
            {
                x += hspace;
                currentWordStart = currentRow.size();
                ++wordsSinceLineStart;
                previousCodePoint = 0;
            }
            else if(codePoint == '\t')
            {
                x += hspace * tabWidth;
                currentWordStart = currentRow.size();
                ++wordsSinceLineStart;
                previousCodePoint = 0;
            }
            else if(codePoint == '\n')
            {
                newLine(currentRow, false);
                currentWordStart = currentRow.size();
                wordsSinceLineStart = 0;
                previousCodePoint = 0;
            }
            else
            {

                auto glyphData = loadGlyphData(codePoint, textSize, currentFontCacheId, *currentTextureCache, *currentMetricsMap, *currentFont);

                if(glyphData)
                {
                    TextureCoordinates texCoords;
                    Glyph::Metrics metrics;

                    std::tie(texCoords, metrics) = *glyphData;
                    float kerning = currentFont->kerning(previousCodePoint, codePoint) * textScale;

                    x += kerning;

                    previousCodePoint = codePoint;

                    float left = metrics.left * textScale;
                    float top = metrics.top * textScale;
                    float width = metrics.width * textScale;
                    float height = metrics.height * textScale;


                    if(x + left + width > textStart.x() + textBorders.size().x())
                    {
                        if(wrapMode == WrapMode::CHARACTERS)
                        {
                            if(currentRow.size() > 0)
                            {
                                newLine(currentRow, true);
                                currentWordStart = currentRow.size();
                                wordsSinceLineStart = 0;
                            }
                        }
                        else if(wrapMode == WrapMode::WORDS)
                        {
                            if(wordsSinceLineStart > 0)
                            {
                                while(currentRow.size() > currentWordStart)
                                {
                                    uint32_t codePointToTransfer = currentRow.back().codePoint;
                                    currentRow.pop_back();
                                    codePoints.insert(codePoints.begin() + codePointIndex, codePointToTransfer);
                                }
                                newLine(currentRow, true);
                                wordsSinceLineStart = 0;
                                currentWordStart = currentRow.size();

                                codePointIndex--;//repeat this codepoint, now on a new line
                                continue;
                            }
                        }
                    }

                    CharacterQuad characterQuad;
                    characterQuad.start = FloatVec2({x + left, y + top});
                    characterQuad.size = FloatVec2({width, height});
                    characterQuad.color = color;
                    characterQuad.texCoords = texCoords;
                    characterQuad.codePoint = codePoint;

                    currentRow.push_back(characterQuad);

                    x += (metrics.advance + characterSpacing) * textScale;
                }
            }
        }

        flushRow(currentRow, false);
    }

    return renderData;
}

template <typename Vec2, typename Rectangle, typename Color>
uint32_t RenderDataGenerator<Vec2, Rectangle, Color>::registerTexture(const Vec2& textureSize)
{
    GIM_ASSERT(textureSize.x() > 0 && textureSize.y() > 0, "trying to register a texture of size (" + std::to_string(textureSize.x()) + "," + std::to_string(textureSize.y()) + "). Both components must be above zero");

    uint32_t newId = mNextTextureId++;
    mTextureSizes[newId] = textureSize;
    return newId;
}

template <typename Vec2, typename Rectangle, typename Color>
template <typename Texture>
uint32_t RenderDataGenerator<Vec2, Rectangle, Color>::registerFontStorage(const std::vector<std::reference_wrapper<Font>>& fonts, const Texture& texture)
{
    uint32_t newImageId = mNextTextureId++;   

    std::shared_ptr<FontCacheEntry> textureCache = std::make_shared<FontCacheEntry>(FontCacheEntry{internal::FontTextureCache(texture), newImageId});
    for(auto& font : fonts)
    {
        uint32_t newFontId = mNextFontId++;   
        GIM_ASSERT(mFontCacheIds.count(font.get().name()) == 0, "trying to register font '" + font.get().name() + "' but it has already been registered");
        mFontCacheIds.emplace(font.get().name(), newFontId);
        mFontCache.emplace(newFontId, textureCache);
        mFonts.emplace(font.get().name(), font);
    }

    return newImageId;
}

template <typename Vec2, typename Rectangle, typename Color>
void RenderDataGenerator<Vec2, Rectangle, Color>::generateQuadWithoutImage(const FloatVec2& position, const FloatVec2& size, const Color& color, float zPosition, std::vector<float>& outPositions, std::vector<float>& outColors)
{
    generateQuadPositions(position, size, zPosition, outPositions);
    generateQuadColors(color, outColors);
}

template <typename Vec2, typename Rectangle, typename Color>
void RenderDataGenerator<Vec2, Rectangle, Color>::generateQuadWithImage(const FloatVec2& position, const FloatVec2& size, const Color& color, float zPosition, const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outPositions, std::vector<float>& outColors, std::vector<float>& outTexCoords, bool flipTexCoords)
{
    generateQuadWithoutImage(position, size, color, zPosition, outPositions, outColors);
    generateQuadTexCoords(texCoordStart, texCoordSize, outTexCoords, flipTexCoords);
}

template <typename Vec2, typename Rectangle, typename Color>
void RenderDataGenerator<Vec2, Rectangle, Color>::generateQuadPositions(const FloatVec2& position, const FloatVec2& size, float zPosition, std::vector<float>& outPositions)
{
    std::vector<float> triangle(
    {
        position.x         ,  position.y         , zPosition,
        position.x         ,  position.y + size.y, zPosition,
        position.x + size.x,  position.y + size.y, zPosition,
        position.x + size.x,  position.y + size.y, zPosition,
        position.x + size.x,  position.y         , zPosition,
        position.x         ,  position.y         , zPosition
    });

    outPositions.insert(outPositions.end(), triangle.begin(), triangle.end());
}

template <typename Vec2, typename Rectangle, typename Color>
void RenderDataGenerator<Vec2, Rectangle, Color>::generateQuadColors(const Color& color, std::vector<float>& outColors)
{
    float r = color.r() / 255.0f;
    float g = color.g() / 255.0f;
    float b = color.b() / 255.0f;
    float a = color.a() / 255.0f;

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

template <typename Vec2, typename Rectangle, typename Color>
void RenderDataGenerator<Vec2, Rectangle, Color>::generateQuadTexCoords(const FloatVec2& texCoordStart, const FloatVec2& texCoordSize, std::vector<float>& outTexCoords, bool flipTexCoords)
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

template <typename Vec2, typename Rectangle, typename Color>
void RenderDataGenerator<Vec2, Rectangle, Color>::generateBorders(const Element& element, const FloatVec2& position, const FloatVec2& size, const Color& color, float zPosition, const FloatVec2& imageSize, std::vector<float>& outPositions, std::vector<float>& outColors, std::vector<float>& outTexCoords)
{
    const BorderMode* borderModePtr = element.findAttribute<BorderMode>("border_mode");
    BorderMode borderMode = borderModePtr ? *borderModePtr : BorderMode::NONE;

    if(borderMode == BorderMode::TOP_BOTTOM || borderMode == BorderMode::FULL)
    {//generate top and bottom borders
        GIM_ASSERT(element.hasAttribute<typename Rectangle::Native>("border_coords_t"), "currentElement set to have borders at top/bottom or full but lacks top border image coords");
        GIM_ASSERT(element.hasAttribute<typename Rectangle::Native>("border_coords_b"), "currentElement set to have borders at top/bottom or full but lacks bottom border image coords");

        const Rectangle& topImageCoords = element.getAttribute<typename Rectangle::Native>("border_coords_t");
        const Rectangle& bottomImageCoords = element.getAttribute<typename Rectangle::Native>("border_coords_b");

        //top quad
        generateQuadWithImage(
                FloatVec2({position.x, position.y - topImageCoords.size().y()}),
                FloatVec2({size.x, (float)topImageCoords.size().y()}),
                color,
                zPosition,
                FloatVec2({(float)topImageCoords.start().x() / imageSize.x, (float)topImageCoords.start().y() / imageSize.y}),
                FloatVec2({(float)topImageCoords.size().x()  / imageSize.x, (float)topImageCoords.size().y()  / imageSize.y}),
                outPositions, outColors, outTexCoords);
        
        //bottom quad
        generateQuadWithImage(
                FloatVec2({position.x, position.y + size.y}),
                FloatVec2({size.x, (float)bottomImageCoords.size().y()}),
                color,
                zPosition,
                FloatVec2({(float)bottomImageCoords.start().x() / imageSize.x, (float)bottomImageCoords.start().y() / imageSize.y}),
                FloatVec2({(float)bottomImageCoords.size().x()  / imageSize.x, (float)bottomImageCoords.size().y()  / imageSize.y}),
                outPositions, outColors, outTexCoords);
    }

    if(borderMode == BorderMode::LEFT_RIGHT || borderMode == BorderMode::FULL)
    {//generate left and right borders
        GIM_ASSERT(element.hasAttribute<typename Rectangle::Native>("border_coords_l"), "currentElement set to have borders at left/right or full but lacks left border image coords");
        GIM_ASSERT(element.hasAttribute<typename Rectangle::Native>("border_coords_r"), "currentElement set to have borders at left/right or full but lacks right border image coords");

        const Rectangle& leftImageCoords = element.getAttribute<typename Rectangle::Native>("border_coords_l");
        const Rectangle& rightImageCoords = element.getAttribute<typename Rectangle::Native>("border_coords_r");

        //left quad
        generateQuadWithImage(
                FloatVec2({position.x - leftImageCoords.size().x(), position.y}),
                FloatVec2({(float)leftImageCoords.size().x(), size.y}),
                color,
                zPosition,
                FloatVec2({(float)leftImageCoords.start().x() / imageSize.x, (float)leftImageCoords.start().y() / imageSize.y}),
                FloatVec2({(float)leftImageCoords.size().x()  / imageSize.x, (float)leftImageCoords.size().y()  / imageSize.y}),
                outPositions, outColors, outTexCoords);
        
        //right quad
        generateQuadWithImage(
                FloatVec2({position.x + size.x, position.y}),
                FloatVec2({(float)rightImageCoords.size().x(), size.y}),
                color,
                zPosition,
                FloatVec2({(float)rightImageCoords.start().x() / imageSize.x, (float)rightImageCoords.start().y() / imageSize.y}),
                FloatVec2({(float)rightImageCoords.size().x()  / imageSize.x, (float)rightImageCoords.size().y()  / imageSize.y}),
                outPositions, outColors, outTexCoords);
    }

    if(borderMode == BorderMode::FULL)
    {//generate corner borders
        GIM_ASSERT(element.hasAttribute<typename Rectangle::Native>("border_coords_tl"), "currentElement set to have full borders but lacks top-left border image coords");
        GIM_ASSERT(element.hasAttribute<typename Rectangle::Native>("border_coords_tr"), "currentElement set to have full borders but lacks top-right border image coords");
        GIM_ASSERT(element.hasAttribute<typename Rectangle::Native>("border_coords_bl"), "currentElement set to have full borders but lacks bottom-left border image coords");
        GIM_ASSERT(element.hasAttribute<typename Rectangle::Native>("border_coords_br"), "currentElement set to have full borders but lacks bottom-right border image coords");

        const Rectangle& topLeftImageCoords = element.getAttribute<typename Rectangle::Native>("border_coords_tl");
        const Rectangle& topRightImageCoords = element.getAttribute<typename Rectangle::Native>("border_coords_tr");
        const Rectangle& bottomLeftImageCoords = element.getAttribute<typename Rectangle::Native>("border_coords_bl");
        const Rectangle& bottomRightImageCoords = element.getAttribute<typename Rectangle::Native>("border_coords_br");

        //topLeft quad
        generateQuadWithImage(
                FloatVec2({position.x - topLeftImageCoords.size().x(), position.y - topLeftImageCoords.size().y()}),
                {(float)topLeftImageCoords.size().x(), (float)topLeftImageCoords.size().y()},
                color,
                zPosition,
                FloatVec2({(float)topLeftImageCoords.start().x() / imageSize.x, (float)topLeftImageCoords.start().y() / imageSize.y}),
                FloatVec2({(float)topLeftImageCoords.size().x()  / imageSize.x, (float)topLeftImageCoords.size().y()  / imageSize.y}),
                outPositions, outColors, outTexCoords);
        
        //topRight quad
        generateQuadWithImage(
                FloatVec2({position.x + size.x, position.y - topRightImageCoords.size().y()}),
                {(float)topRightImageCoords.size().x(), (float)topRightImageCoords.size().y()},
                color,
                zPosition,
                FloatVec2({(float)topRightImageCoords.start().x() / imageSize.x, (float)topRightImageCoords.start().y() / imageSize.y}),
                FloatVec2({(float)topRightImageCoords.size().x()  / imageSize.x, (float)topRightImageCoords.size().y()  / imageSize.y}),
                outPositions, outColors, outTexCoords);

        //bottomLeft quad
        generateQuadWithImage(
                FloatVec2({position.x - bottomLeftImageCoords.size().y(), position.y + size.y}),
                {(float)bottomLeftImageCoords.size().x(), (float)bottomLeftImageCoords.size().y()},
                color,
                zPosition,
                FloatVec2({(float)bottomLeftImageCoords.start().x() / imageSize.x, (float)bottomLeftImageCoords.start().y() / imageSize.y}),
                FloatVec2({(float)bottomLeftImageCoords.size().x()  / imageSize.x, (float)bottomLeftImageCoords.size().y()  / imageSize.y}),
                outPositions, outColors, outTexCoords);
        
        //bottomRight quad
        generateQuadWithImage(
                FloatVec2({position.x + size.x, position.y + size.y}),
                {(float)bottomRightImageCoords.size().x(), (float)bottomRightImageCoords.size().y()},
                color,
                zPosition,
                FloatVec2({(float)bottomRightImageCoords.start().x() / imageSize.x, (float)bottomRightImageCoords.start().y() / imageSize.y}),
                FloatVec2({(float)bottomRightImageCoords.size().x()  / imageSize.x, (float)bottomRightImageCoords.size().y()  / imageSize.y}),
                outPositions, outColors, outTexCoords);
    }
}

template <typename Vec2, typename Rectangle, typename Color>
float RenderDataGenerator<Vec2, Rectangle, Color>::getHSpace(const Font& font, uint32_t size)
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

        GIM_ASSERT(glyphPtr != nullptr, "font " + font.name() + " didn't contain codepoint for ' ' (whitespace)");

        fontCache.textureCoordinates.add(*glyphPtr, fontId);
        fontCache.metrics.emplace(CodePointSizeId({whitespace, size, fontId}), glyphPtr->metrics);

        return glyphPtr->metrics.advance;
    }
}

template <typename Vec2, typename Rectangle, typename Color>
std::unique_ptr<std::tuple<TextureCoordinates, Glyph::Metrics>> RenderDataGenerator<Vec2, Rectangle, Color>::loadGlyphData(uint32_t codePoint, uint32_t textSize, uint32_t fontCacheId, internal::FontTextureCache& textureCache, MetricsMap& metricsMap, const Font& font)
{
    auto texCoordsPtr = textureCache.glyphCoords(codePoint, textSize, fontCacheId);
    TextureCoordinates texCoords;
    Glyph::Metrics metrics;

    if(texCoordsPtr == nullptr)
    {
        auto glyphPtr = font.generateGlyph(codePoint);

        if(glyphPtr)
        {
            metrics = glyphPtr->metrics;
            texCoords = textureCache.add(*glyphPtr, fontCacheId);
            metricsMap.emplace(CodePointSizeId({codePoint, textSize, fontCacheId}), metrics);
        }
        else
        {//codepoint doesn't exist - do nothing
            return nullptr;
        }
    }
    else
    {
        GIM_ASSERT(metricsMap.count(CodePointSizeId({codePoint, textSize, fontCacheId})) != 0, "glyph existed in texture but not in metrics map. this is a bug.");
        texCoords = *texCoordsPtr;
        metrics = metricsMap.at(CodePointSizeId({codePoint, textSize, fontCacheId}));
    }

    return std::unique_ptr<std::tuple<TextureCoordinates, Glyph::Metrics>>(new std::tuple<TextureCoordinates, Glyph::Metrics>(texCoords, metrics));
}
