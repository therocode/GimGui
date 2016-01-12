#pragma once
#include <glutils/buffer.hpp>
#include <glutils/vao.hpp>
#include <glutils/shader.hpp>
#include <array>
#include <fstream>
#include <glutils/texture.hpp>
#include <gimgui/logic/renderdatagenerator.hpp>
#include <helpers/vector2.hpp>
#include <helpers/rectangle.hpp>
#include <helpers/color.hpp>

union SDL_Event;

class SimpleRendering
{

    public:
        SimpleRendering(const Vector2& viewSize);
        void loop();
        void setViewSize(const Vector2& viewSize);
        bool isTerminated() const;
        void handleEvents(const std::deque<SDL_Event>& events);
    private:
        void quit();
        bool mQuit;
        Buffer mTriangles;
        Buffer mColors;
        Buffer mTexCoords;
        Shader mBaseShader;
        std::array<float, 16> mProjection;
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> mRenderDataGenerator;
        Vector2 mLastPosition;
        std::vector<std::unique_ptr<Texture>> mTextures;
        std::unordered_map<uint32_t, std::reference_wrapper<const Texture>> mTextureHandleIndex;
        std::ifstream mFontFile;
        std::ifstream mBoldFontFile;
        std::ifstream mItalicFontFile;
        std::ifstream mBoldItalicFontFile;
        gim::Font mFont;
        gim::Font mBoldFont;
        gim::Font mItalicFont;
        gim::Font mBoldItalicFont;
        gim::Element mRoot;

        GLint mPositionLocation;
        GLint mColorLocation;
        GLint mTexCoordLocation;

        Vector2 mViewSize;
};
