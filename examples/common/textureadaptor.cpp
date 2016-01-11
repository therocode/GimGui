#include <textureadaptor.hpp>
#include <iostream>
        
TextureAdaptor::TextureAdaptor(Texture& texture):
    mTexture(texture)
{
}

void TextureAdaptor::initialize(uint32_t width, uint32_t height)
{
    mTexture.create(width, height, Color(0, 0, 0, 0), false, true);
}

void TextureAdaptor::resize(uint32_t width, uint32_t height)
{
    mTexture.resize(width, height);
}

void TextureAdaptor::writeBitmap(uint32_t xPos, uint32_t yPos, const gim::BitMap& bitMap)
{
    for(uint32_t x = 0; x < bitMap.width; x++)
    {
        for(uint32_t y = 0; y < bitMap.height; y++)
        {
            uint8_t value = bitMap.pixels[x + y * bitMap.width];

            if(value != 0)
                mTexture.setPixel(xPos + x, yPos + y, Color(value, value, value, value));
        }
    }

    mTexture.update();
}

uint32_t TextureAdaptor::handle() const
{
    return mTexture.getId();
}
