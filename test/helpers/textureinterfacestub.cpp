#include <helpers/textureinterfacestub.hpp>

void TextureInterfaceStub::initialize(uint32_t width, uint32_t height)
{
    //initialize a texture to the given size
}

void TextureInterfaceStub::resize(uint32_t width, uint32_t height)
{
    //resize a texturem keeping all old data intact
}

void TextureInterfaceStub::writeBitmap(uint32_t x, uint32_t y, const gim::BitMap& bitMap)
{
    //write the bitmap at the specific location given. The bitmap is in 8 bit grayscale format.
}
