#pragma once
#include <glutils/buffer.hpp>
#include <glutils/vao.hpp>
#include <glutils/shader.hpp>
#include <array>

struct Vec2;

class SimpleRendering
{

    public:
        SimpleRendering(const Vec2& viewSize);
        void loop();
        void setViewSize(const Vec2& viewSize);
    private:
        VAO mVao;
        Buffer mTriangles;
        Buffer mColors;
        Shader mBaseShader;
        std::array<float, 16> mProjection;
};
