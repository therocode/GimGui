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
        gim::Element({"container", "yellow"}, {},
        {
            gim::Element({"child", "green"}),
            gim::Element({"child", "blue"}),
            gim::Element({"child", "red"})
        })
    });

    gim::AttributePopulator populator;
    populator.addConfiguration("position", Vec2({0, 0}));
    populator.addConfiguration("size", Vec2({10, 10}));
    populator.populate(gui.root());

    gui.root().createAttribute("color", Color({255, 255, 0}));
    gui.root().children()[0]->createAttribute("color", Color({0, 255, 0}));
    gui.root().children()[1]->createAttribute("color", Color({0, 0, 255}));
    gui.root().children()[2]->createAttribute("color", Color({255, 0, 0}));
    gui.root().setAttribute("position", Vec2({1, 1}));
    gui.root().children()[0]->setAttribute("position", Vec2({3, 3}));
    gui.root().children()[1]->setAttribute("position", Vec2({6, 6}));
    gui.root().children()[2]->setAttribute("position", Vec2({9, 9}));
    gui.root().setAttribute("size", Vec2({8, 8}));
    gui.root().children()[0]->setAttribute("size", Vec2({5, 5}));
    gui.root().children()[1]->setAttribute("size", Vec2({5, 5}));
    gui.root().children()[2]->setAttribute("size", Vec2({5, 5}));

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
