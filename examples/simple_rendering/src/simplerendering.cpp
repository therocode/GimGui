#include "simplerendering.hpp"
#include <glutils/baseshader.hpp>
#include <gl_core_3_3.h>
#include <iostream>

SimpleRendering::SimpleRendering():
    mTriangles(Buffer::ARRAY_BUFFER),
    mColors(Buffer::ARRAY_BUFFER)
{
    //rendering
    mVao.bind();

    mTriangles.bind();

    mTriangles.setData(std::vector<float>(
    {
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f
    }));

    mColors.bind();

    mColors.setData(std::vector<float>(
    {
         1.0f,  1.0f, 0.0f,
         1.0f,  0.0f, 1.0f,
         0.0f,  1.0f, 1.0f 
    }));

    mVao.setVertexAttribute(0, 3, mTriangles);
    mVao.setVertexAttribute(1, 3, mColors);

    mVao.unbind();

    mBaseShader.setSource(BaseShader::vertexSource, BaseShader::fragmentSource);
    mBaseShader.compile();

    //gui
}

void SimpleRendering::loop()
{
    mVao.bind();
    mBaseShader.activate();

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
