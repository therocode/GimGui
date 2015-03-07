#include "simplerendering.hpp"
#include <glutils/baseshader.hpp>
#include <gl_core_3_3.h>
#include <iostream>
#include <gimgui/gui/gui.hpp>
#include <gimgui/logic/attributepopulator.hpp>
#include <gimgui/logic/allpropagator.hpp>
#include <vec2.hpp>
#include <color.hpp>

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
    gim::Gui gui(
    {
        gim::Element({"container"},
        {
            {"color",    Color({140, 35, 24})},
            {"position", Vec2({1, 1})},
            {"size",     Vec2({8, 8})}
        },
        {
            gim::Element({"child"},
            {
                {"color",    Color({94, 140, 106})},
                {"position", Vec2({3, 3})},
                {"size",     Vec2({5, 5})}
            }),
            gim::Element({"child"},
            {
                {"color",    Color({136, 166, 94})},
                {"position", Vec2({5, 5})},
                {"size",     Vec2({5, 5})}
            }),
            gim::Element({"child"},
            {
                {"color",    Color({191, 179, 90})},
                {"position", Vec2({7, 7})},
                {"size",     Vec2({5, 5})}
            })
        })
    });

    gim::AllPropagator all(gui.root());

    std::vector<float> triangles;
    std::vector<float> colors;
    gim::Element* element;
    while((element = all.next()))
    {
        const Vec2& position = element->getAttribute<Vec2>("position");
        const Vec2& size = element->getAttribute<Vec2>("size");
        std::vector<float> triangle(
        {
            position.x / 10.0f                  ,  position.y / 10.0f                  , 0.0f,
            position.x / 10.0f                  ,  position.y / 10.0f - size.y / 10.0f , 0.0f,
            position.x / 10.0f + size.x / 10.0f ,  position.y / 10.0f - size.y / 10.0f , 0.0f,
            position.x / 10.0f                  ,  position.y / 10.0f                  , 0.0f,
            position.x / 10.0f + size.x / 10.0f ,  position.y / 10.0f                  , 0.0f,
            position.x / 10.0f + size.x / 10.0f ,  position.y / 10.0f - size.y / 10.0f , 0.0f
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
}

void SimpleRendering::loop()
{
    mVao.bind();
    mBaseShader.activate();

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, mTriangles.getElementAmount());
}
