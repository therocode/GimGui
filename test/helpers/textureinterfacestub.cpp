#include <helpers/textureinterfacestub.hpp>

TextureInterfaceStub::TextureInterfaceStub(const Vec2& size):
    mSize(size)
{
}

void TextureInterfaceStub::initialize(uint32_t width, uint32_t height)
{
    //initialize a texture to the given size
}

void TextureInterfaceStub::resize(uint32_t width, uint32_t height)
{
    //resize a texturem keeping all old data intact
    mSize.x = width;
    mSize.y = height;
}

void TextureInterfaceStub::writeBitmap(uint32_t x, uint32_t y, const gim::BitMap& bitMap)
{
    //write the bitmap at the specific location given. The bitmap is in 8 bit grayscale format.
}

Vec2 TextureInterfaceStub::size() const
{
    return mSize;
}
