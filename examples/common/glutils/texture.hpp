#pragma once
#include <stdint.h>
#include <opengl.hpp>
#include <helpers/color.hpp>
#include <memory>
#include <helpers/vector2.hpp>

class Texture
{
    public:
        Texture();
        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;
        Texture(Texture&& other);
        Texture& operator=(Texture&& other);
        GLuint getId() const;
        void create(uint32_t width, uint32_t height, const uint8_t* imageData, bool smooth = false, bool interactive = false);
        void create(uint32_t width, uint32_t height, const Color& color, bool smooth = false, bool interactive = false);
        void resize(uint32_t width, uint32_t height);
        Vector2 size() const;
        void destroy();
        void setPixel(uint32_t x, uint32_t y, const Color& color);
        Color getPixel(uint32_t x, uint32_t y) const;
        uint8_t* getPixelData();
        const uint8_t* getPixelData() const;
        void update();
        ~Texture();
    private:
        GLuint mId;
        uint32_t mWidth;
        uint32_t mHeight;
        bool mInteractive;
        std::unique_ptr<uint8_t[]> pixelData;
};
