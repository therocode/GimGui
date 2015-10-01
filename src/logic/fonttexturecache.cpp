#include <gimgui/logic/fonttexturecache.hpp>
#include <algorithm>

namespace gim
{
    namespace internal
    {
        std::unique_ptr<TextureCoordinates> FontTextureCache::glyphCoords(uint32_t codePoint, uint32_t size, uint32_t fontId)
        {
            auto glyphRectangleIterator = mGlyphRectangles.find({codePoint, size, fontId});

            if(glyphRectangleIterator != mGlyphRectangles.end())
            {
                auto rectangle = glyphRectangleIterator->second.first;
                bool flipped = glyphRectangleIterator->second.second;

                return std::unique_ptr<TextureCoordinates>(new TextureCoordinates(generateTexCoords(rectangle, flipped)));
            }
            else
            {
                return nullptr;
            }
        }

        TextureCoordinates FontTextureCache::add(const Glyph& glyph, uint32_t fontId)
        {
            uint32_t width = glyph.image.width;
            uint32_t height = glyph.image.height;
            
            if(width > 0 && height > 0)
            {
                auto rectangle = mGlyphPacker.insert({(int32_t)width, (int32_t)height});
                bool flipped = rectangle.size.x == height && rectangle.size.y == width;

                uint32_t neededX = rectangle.start.x + std::max(rectangle.size.x, rectangle.size.y); //using both x and y takes flipping into account
                uint32_t neededY = rectangle.start.y + std::max(rectangle.size.x, rectangle.size.y); 
                if(neededX >= mCurrentSize || neededY >= mCurrentSize)
                {//time to resize texture
                    uint32_t neededSize = std::max(neededX, neededY);

                    while(mCurrentSize < neededSize)
                    {
                        mCurrentSize *= 2;
                        mResizeStorage(mCurrentSize, mCurrentSize);
                    }
                }

                mGlyphRectangles.emplace(CodePointSizeId({glyph.codePoint, glyph.size, fontId}), std::pair<Rectangle, bool>({rectangle, flipped}));

                mWriteBitmap(rectangle.start.x, rectangle.start.y, flipped ? flipBitMap(glyph.image) : glyph.image);

                return generateTexCoords(rectangle, flipped);
            }

            return {0.0f, 0.0f, 0.0f, 0.0f};
        }

        TextureCoordinates FontTextureCache::generateTexCoords(const Rectangle rectangle, bool flipped) const
        {
            TextureCoordinates coordinates;

            coordinates.xStart = (float)rectangle.start.x / mCurrentSize;
            coordinates.yStart = (float)rectangle.start.y / mCurrentSize;
            coordinates.xEnd = coordinates.xStart + (float) rectangle.size.x / mCurrentSize;
            coordinates.yEnd = coordinates.yStart + (float) rectangle.size.y / mCurrentSize;
            coordinates.flipped = flipped;

            return coordinates;
        }
                
        BitMap FontTextureCache::flipBitMap(const BitMap& bitMap) const
        {
            BitMap flipped;
            flipped.width = bitMap.height;
            flipped.height = bitMap.width;
            flipped.pixels.resize(flipped.width * flipped.height);

            for(uint32_t y = 0; y < bitMap.height; ++y)
            {
                for(uint32_t x = 0; x < bitMap.width; ++x)
                {
                    flipped.pixels[y + (flipped.height - x - 1) * flipped.width] = bitMap.pixels[x + y * bitMap.width];
                }
            }

            return flipped;
        }
        
        TextureCoordinates FontTextureCache::solidCoords() const
        {
            auto texCoords = generateTexCoords(mSolidCoords, false);
            return texCoords;
        }
    }
}
