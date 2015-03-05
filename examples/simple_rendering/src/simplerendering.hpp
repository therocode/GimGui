#pragma once
#include <glutils/buffer.hpp>
#include <glutils/vao.hpp>
#include <glutils/shader.hpp>

class SimpleRendering
{
    public:
        SimpleRendering();
        void loop();
    private:
        VAO mVao;
        Buffer mTriangles;
        Buffer mColors;
        Shader mBaseShader;
};
