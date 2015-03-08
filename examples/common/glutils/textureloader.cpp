#include <glutils/textureloader.hpp>

Texture loadTexture(const std::string& path)
{
    uint32_t width;
    uint32_t height;

    std::vector<unsigned char> image; //the raw pixels

    //decode
    unsigned error = lodepng::decode(image, width, height, path);

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
    Texture texture;
    texture.create(width, height, &image[0]);
    return texture;
}
