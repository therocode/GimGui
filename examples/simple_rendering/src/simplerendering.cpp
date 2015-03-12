#include "simplerendering.hpp"
#include <glutils/baseshader.hpp>
#include <glutils/uniform.hpp>
#include <gl_core_3_3.h>
#include <iostream>
#include <glutils/projection.hpp>
#include <SDL2/SDL.h>
#include <window.hpp>
#include "events.hpp"
#include <glutils/textureloader.hpp>
#include <gimgui/logic/boundarypropagator.hpp>

Callback<> setClickColor = [] (gim::Element& self) {self.setAttribute("color", self.getAttribute<Color>("click_color"));};
Callback<> setOriginalColor = [] (gim::Element& self) {self.setAttribute("color", self.getAttribute<Color>("original_color"));};

SimpleRendering::SimpleRendering(const Vec2& viewSize):
    mQuit(false),
    mTriangles(Buffer::ARRAY_BUFFER),
    mColors(Buffer::ARRAY_BUFFER),
    mTexCoords(Buffer::ARRAY_BUFFER),
    mLastPosition({-1000, -1000}),
    mRoot({"container"},
        {
            {"color",    Color(140, 35, 24)},
            {"original_color",    Color(140, 35, 24)},
            {"click_color",    Color(174, 70, 56)},
            {"position", Vec2({200, 150})},
            {"size",     Vec2({256, 256})},
            {"stretch_mode", gim::StretchMode::STRETCHED},
            {"image_id", 0},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({64, 64}))},
            {"on_click", setClickColor},
            {"on_mouse_release", setOriginalColor}
        },
        {
            gim::Element({"child"},
            {
                {"color",    Color(94, 140, 106)},
                {"original_color",    Color(94, 140, 106)},
                {"click_color",    Color(124, 170, 136)},
                {"position", Vec2({20, 20})},
                {"size",     Vec2({64, 64})},
                {"image_id", 0},
                {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({64, 64}))},
                {"block_event", true},
                {"on_click", setClickColor},
                {"on_mouse_release", setOriginalColor}
            }),
            gim::Element({"child"},
            {
                {"color",    Color(136, 166, 94)},
                {"original_color",    Color(136, 166, 94)},
                {"click_color",    Color(166, 196, 124)},
                {"position", Vec2({90, 20})},
                {"size",     Vec2({64, 64})},
                {"image_id", 0},
                {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({64, 64}))},
                {"on_click", setClickColor},
                {"on_mouse_release", setOriginalColor},
                {"block_event", true}
            }),
            gim::Element({"child"},
            {
                {"color",    Color(191, 179, 90)},
                {"original_color",    Color(191, 179, 90)},
                {"click_color",    Color(221, 209, 124)},
                {"position", Vec2({20, 90})},
                {"size",     Vec2({64, 64})},
                {"image_id", 0},
                {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({64, 64}))},
                {"on_click", setClickColor},
                {"on_mouse_release", setOriginalColor}
            })
        })
{
    //load textures

    mTextures.emplace(0, loadTexture("examples/resources/xpattern.png"));

    mRenderDataGenerator.registerImageInfo(0, {64, 64});
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
    mVao.setVertexAttribute(2, 2, mTexCoords);

    mVao.unbind();

    mBaseShader.setSource(BaseShader::vertexSource, BaseShader::fragmentSource);
    mBaseShader.compile();

    Projection proj;
    mProjection = proj.createOrthoProjection(0.0f, (GLfloat)viewSize.x, 0.0f, (GLfloat)viewSize.y, 0.000000001f, 100.0f);
}

void SimpleRendering::loop()
{
    //rendering
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    mVao.bind();
    mBaseShader.activate();
    mBaseShader.setUniform("projection", UniformType::MAT4X4, &mProjection[0]);

    auto renderDatas = mRenderDataGenerator.generate(mRoot);

    for(auto renderData : renderDatas)
    {
        GIM_ASSERT(mTextures.count(renderData.imageId) != 0, "invalid texture given");
        mBaseShader.setUniform("texture", UniformType::TEXTURE, &mTextures.at(renderData.imageId));
        mTriangles.setData(renderData.positions);
        mColors.setData(renderData.colors);
        mTexCoords.setData(renderData.texCoords);

        glDrawArrays(GL_TRIANGLES, 0, mTriangles.getElementAmount());
    }
}

void SimpleRendering::setViewSize(const Vec2& viewSize)
{
    Projection proj;
    mProjection = proj.createOrthoProjection(0.0f, (GLfloat)viewSize.x, 0.0f, (GLfloat)viewSize.y, 0.000000001f, 100.0f);
    glViewport(0, 0, viewSize.x, viewSize.y);
}

bool SimpleRendering::isTerminated() const
{
    return mQuit;
}

void SimpleRendering::handleEvents(const std::deque<SDL_Event>& events)
{
    for(const auto& event : events)
    {
        if(event.type == SDL_WINDOWEVENT)
        {
            if(event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                setViewSize(Vec2({event.window.data1, event.window.data2}));
            }
        }
        else if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit();
            }
        }
        else if(event.type == SDL_MOUSEMOTION)
        {
            Vec2 position = Vec2({event.motion.x, event.motion.y});
            moveMouse(mRoot, position, mLastPosition);
            mLastPosition = position;
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            Vec2 position = Vec2({event.button.x, event.button.y});
            click(mRoot, position);
        }
        else if(event.type == SDL_MOUSEBUTTONUP)
        {
            Vec2 position = Vec2({event.button.x, event.button.y});
            mouseRelease(mRoot, position);
        }
    }
}

void SimpleRendering::quit()
{
    mQuit = true;
}
