#include "simplerendering.hpp"
#include <glutils/baseshader.hpp>
#include <glutils/uniform.hpp>
#include <gl_core_3_3.h>
#include <iostream>
#include <gimgui/gui/gui.hpp>
#include <gimgui/logic/attributepopulator.hpp>
#include <gimgui/logic/allpropagator.hpp>
#include <gimgui/logic/absolutemap.hpp>
#include <vec2.hpp>
#include <color.hpp>
#include <glutils/projection.hpp>
#include <GLFW/glfw3.h>
#include <window.hpp>

SimpleRendering::SimpleRendering(const Vec2& viewSize):
    mQuit(false),
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
    gim::Gui gui(
    {
        gim::Element({"container"},
        {
            {"color",    Color({140, 35, 24})},
            {"position", Vec2({200, 150})},
            {"size",     Vec2({400, 300})}
        },
        {
            gim::Element({"child"},
            {
                {"color",    Color({94, 140, 106})},
                {"position", Vec2({20, 20})},
                {"size",     Vec2({50, 50})}
            }),
            gim::Element({"child"},
            {
                {"color",    Color({136, 166, 94})},
                {"position", Vec2({90, 20})},
                {"size",     Vec2({50, 50})}
            }),
            gim::Element({"child"},
            {
                {"color",    Color({191, 179, 90})},
                {"position", Vec2({20, 90})},
                {"size",     Vec2({120, 50})}
            })
        })
    });

    gim::AllPropagator all(gui.root());

    std::vector<float> triangles;
    std::vector<float> colors;
    gim::Element* element;

    gim::AbsoluteMap<Vec2> absolutePositions("position");

    while((element = all.next()))
    {
        const Vec2& position = absolutePositions.getAbsoluteOf(*element);
        const Vec2& size = element->getAttribute<Vec2>("size");
        std::vector<float> triangle(
        {
            (float)position.x         ,  (float)position.y         , 0.0f,
            (float)position.x         ,  (float)position.y + size.y, 0.0f,
            (float)position.x + size.x,  (float)position.y + size.y, 0.0f,
            (float)position.x         ,  (float)position.y         , 0.0f,
            (float)position.x + size.x,  (float)position.y         , 0.0f,
            (float)position.x + size.x,  (float)position.y + size.y, 0.0f
        });

        triangles.insert(triangles.end(), triangle.begin(), triangle.end());

        const Color& color = element->getAttribute<Color>("color");

        std::vector<float> colorList(
        {
            color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 
            color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 
            color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
            color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
            color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
            color.r / 255.0f, color.g / 255.0f, color.b / 255.0f
        });

        colors.insert(colors.end(), colorList.begin(), colorList.end());
    }

    mTriangles.setData(triangles);
    mColors.setData(colors);

    Projection proj;
    mProjection = proj.createOrthoProjection(0.0f, (GLfloat)viewSize.x, 0.0f, (GLfloat)viewSize.y, 0.000000001f, 100.0f);
}

void SimpleRendering::loop()
{
    mVao.bind();
    mBaseShader.activate();
    mBaseShader.setUniform("projection", UniformType::MAT4X4, &mProjection[0]);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, mTriangles.getElementAmount());
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
    }
}

void SimpleRendering::quit()
{
    mQuit = true;
}
