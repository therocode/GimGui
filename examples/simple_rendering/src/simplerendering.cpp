#include "simplerendering.hpp"
#include <glutils/baseshader.hpp>
#include <glutils/uniform.hpp>
#include <gl_core_3_3.h>
#include <iostream>
#include <gimgui/logic/renderdatagenerator.hpp>
#include <helpers/vec2.hpp>
#include <helpers/color.hpp>
#include <glutils/projection.hpp>
#include <GLFW/glfw3.h>
#include <window.hpp>
#include "events.hpp"

SimpleRendering::SimpleRendering(const Vec2& viewSize):
    mQuit(false),
    mTriangles(Buffer::ARRAY_BUFFER),
    mColors(Buffer::ARRAY_BUFFER),
    mGui(
    {
        gim::Element({"container", "random"},
        {
            {"color",    Color({140, 35, 24})},
            {"position", Vec2({200, 150})},
            {"size",     Vec2({400, 300})}
        },
        {
            gim::Element({"child", "random"},
            {
                {"color",    Color({94, 140, 106})},
                {"position", Vec2({20, 20})},
                {"size",     Vec2({50, 50})}
            }),
            gim::Element({"child", "random"},
            {
                {"color",    Color({136, 166, 94})},
                {"position", Vec2({90, 20})},
                {"size",     Vec2({50, 50})}
            }),
            gim::Element({"child", "random"},
            {
                {"color",    Color({191, 179, 90})},
                {"position", Vec2({20, 90})},
                {"size",     Vec2({120, 50})}
            })
        })
    })
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

    Projection proj;
    mProjection = proj.createOrthoProjection(0.0f, (GLfloat)viewSize.x, 0.0f, (GLfloat)viewSize.y, 0.000000001f, 100.0f);
}

void SimpleRendering::loop()
{
    mGui.sendEvent<gim::AllPropagator>(randomColorEvent());

    //rendering
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    mVao.bind();
    mBaseShader.activate();
    mBaseShader.setUniform("projection", UniformType::MAT4X4, &mProjection[0]);

    gim::RenderDataGenerator<Vec2, Color> generator;
    auto renderDatas = generator.generate(mGui.root());

    for(auto renderData : renderDatas)
    {
        mTriangles.setData(renderData.positions);
        mColors.setData(renderData.colors);

        glDrawArrays(GL_TRIANGLES, 0, mTriangles.getElementAmount());
    }
}

void SimpleRendering::setViewSize(const Vec2& viewSize)
{
    Projection proj;
    mProjection = proj.createOrthoProjection(0.0f, (GLfloat)viewSize.x, 0.0f, (GLfloat)viewSize.y, 0.000000001f, 100.0f);
}

void SimpleRendering::keyEvent(int32_t key)
{
    if(key == GLFW_KEY_ESCAPE)
        quit();
}

bool SimpleRendering::isTerminated() const
{
    return mQuit;
}

void SimpleRendering::handleEvents(const Events& events)
{
    for(auto newSize : events.resizeEvents)
    {
        setViewSize(newSize);
    }

    for(int32_t key : events.keyEvents)
    {
        if(key == GLFW_KEY_ESCAPE)
            quit();
        else if(key == GLFW_KEY_D)
        {
            Vec2 pos = mGui.root().getAttribute<Vec2>("position") + Vec2({20, 20});
            mGui.root().setAttribute("position", pos);
        }
    }
}

void SimpleRendering::quit()
{
    mQuit = true;
}
