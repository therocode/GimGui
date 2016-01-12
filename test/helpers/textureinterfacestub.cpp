#include <helpers/textureinterfacestub.hpp>

TextureInterfaceStub::TextureInterfaceStub(const Vector2& size, uint32_t handle):
    mSize(size),
    mHandle(handle)
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

Vector2 TextureInterfaceStub::size() const
{
    return mSize;
}

uint32_t TextureInterfaceStub::handle() const
{
    return mHandle;
}
