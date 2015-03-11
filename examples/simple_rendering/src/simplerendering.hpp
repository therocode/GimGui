#pragma once
#include <glutils/buffer.hpp>
#include <glutils/vao.hpp>
#include <glutils/shader.hpp>
#include <array>
#include <gimgui/gui/gui.hpp>
#include <glutils/texture.hpp>
#include <gimgui/logic/renderdatagenerator.hpp>
#include <helpers/vec2.hpp>
#include <helpers/color.hpp>

struct Vec2;
struct Events;

class SimpleRendering
{

    public:
        SimpleRendering(const Vec2& viewSize);
        void loop();
        void setViewSize(const Vec2& viewSize);
        void keyEvent(int32_t key);
        bool isTerminated() const;
        void handleEvents(const Events& events);
    private:
        void quit();
        bool mQuit;
        VAO mVao;
        Buffer mTriangles;
        Buffer mColors;
        Buffer mTexCoords;
        Shader mBaseShader;
        std::array<float, 16> mProjection;
        gim::RenderDataGenerator<Vec2, Color> mRenderDataGenerator;
        Vec2 mLastPosition;
        gim::Gui mGui;
        std::unordered_map<int32_t, Texture> mTextures;
};
