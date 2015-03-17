#include "simplerendering.hpp"
#include <glutils/baseshader.hpp>
#include <glutils/uniform.hpp>
#include <opengl.hpp>
#include <iostream>
#include <glutils/projection.hpp>
#include <SDL2/SDL.h>
#include <window.hpp>
#include "events.hpp"
#include <glutils/textureloader.hpp>
#include <gimgui/logic/boundarypropagator.hpp>

Callback<Vec2> setClickColor = [] (gim::Element& self, const Vec2& clickPos) {self.setAttribute("color", self.getAttribute<Color>("click_color"));};
Callback<> setOriginalColor = [] (gim::Element& self)
{
    self.setAttribute("color", self.getAttribute<Color>("original_color"));
    self.setAttribute("dragged", false);
    self.setAttribute("resize", false);
};
Callback<Vec2> printClicked = [] (gim::Element& self, const Vec2& clickPos)
{
    std::cout << self.getAttribute<std::string>("name") << " clicked\n";
};
Callback<Vec2> toggleStretchMode = [] (gim::Element& self, const Vec2& clickPos)
{
    gim::StretchMode currentStretchMode = self.getAttribute<gim::StretchMode>("stretch_mode");
    gim::StretchMode newStretchMode;

    gim::AbsoluteMap<Vec2> absoluteMap("position");

    const Vec2 position = absoluteMap.getAbsoluteOf(self);
    Vec2 relativePos;
    relativePos.x = clickPos.x - position.x;
    relativePos.y = clickPos.y - position.y;
    const Vec2 size = self.getAttribute<Vec2>("size");

    if(relativePos.x > size.x - 20 && relativePos.y > size.y - 20)
    {
        self.setAttribute("resize", true);
    }
    else
    {
        if(currentStretchMode == gim::StretchMode::STRETCHED)
            newStretchMode = gim::StretchMode::TILED;
        else if(currentStretchMode == gim::StretchMode::TILED)
            newStretchMode = gim::StretchMode::V_TILED;
        else if(currentStretchMode == gim::StretchMode::V_TILED)
            newStretchMode = gim::StretchMode::H_TILED;
        else if(currentStretchMode == gim::StretchMode::H_TILED)
            newStretchMode = gim::StretchMode::STRETCHED;

        self.setAttribute("stretch_mode", newStretchMode);
    }

    self.setAttribute("dragged", true);
};

SimpleRendering::SimpleRendering(const Vec2& viewSize):
    mQuit(false),
    mTriangles(Buffer::ARRAY_BUFFER, Buffer::DYNAMIC),
    mColors(Buffer::ARRAY_BUFFER, Buffer::DYNAMIC),
    mTexCoords(Buffer::ARRAY_BUFFER, Buffer::DYNAMIC),
    mLastPosition({-1000, -1000}),
    mRoot({"container"},
        {
            {"name" , std::string("red")},
            {"color",    Color(140, 35, 24)},
            {"original_color",    Color(140, 35, 24)},
            {"click_color",    Color(174, 70, 56)},
            {"position", Vec2({200, 150})},
            {"size",     Vec2({256, 256})},
            {"stretch_mode", gim::StretchMode::STRETCHED},
            {"image_id", 0},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))},
            {"dragged", false},
            {"resize", false},
            {"on_click", CallbackList<Vec2>({setClickColor, printClicked, toggleStretchMode})},
            {"on_mouse_release", setOriginalColor}
        },
        {
            gim::Element({"child"},
            {
                {"name" , std::string("blue")},
                {"color",    Color(94, 140, 106)},
                {"original_color",    Color(94, 140, 106)},
                {"click_color",    Color(124, 170, 136)},
                {"position", Vec2({20, 20})},
                {"size",     Vec2({64, 64})},
                {"stretch_mode", gim::StretchMode::STRETCHED},
                {"image_id", 0},
                {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))},
                {"block_event", true},
                {"dragged", false},
                {"resize", false},
                {"on_click", CallbackList<Vec2>({setClickColor, printClicked, toggleStretchMode})},
                {"on_mouse_release", setOriginalColor}
            }),
            gim::Element({"child"},
            {
                {"name" , std::string("green")},
                {"color",    Color(136, 166, 94)},
                {"original_color",    Color(136, 166, 94)},
                {"click_color",    Color(166, 196, 124)},
                {"position", Vec2({90, 20})},
                {"size",     Vec2({64, 64})},
                {"stretch_mode", gim::StretchMode::STRETCHED},
                {"image_id", 0},
                {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({4, 4}))},
                {"dragged", false},
                {"resize", false},
                {"on_click", CallbackList<Vec2>({setClickColor, printClicked, toggleStretchMode})},
                {"on_mouse_release", setOriginalColor},
                {"block_event", true}
            }),
            gim::Element({"child"},
            {
                {"name" , std::string("yellow")},
                {"color",    Color(191, 179, 90)},
                {"original_color",    Color(191, 179, 90)},
                {"click_color",    Color(221, 209, 124)},
                {"position", Vec2({20, 90})},
                {"size",     Vec2({64, 64})},
                {"stretch_mode", gim::StretchMode::STRETCHED},
                {"image_id", 0},
                {"dragged", false},
                {"resize", false},
                {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))},
                {"on_click", CallbackList<Vec2>({setClickColor, printClicked, toggleStretchMode})},
                {"on_mouse_release", setOriginalColor},
                {"block_event", true}
            })
        })
{
    //load textures

    mTextures.emplace(0, loadTexture("resources/xpattern.png"));

    mRenderDataGenerator.registerImageInfo(0, {32, 32});
    //rendering

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

    //mVao.setVertexAttribute(0, 3, mTriangles);
    //mVao.setVertexAttribute(1, 3, mColors);
    //mVao.setVertexAttribute(2, 2, mTexCoords);

    //mVao.unbind();

    mBaseShader.setSource(BaseShader::vertexSource, BaseShader::fragmentSource);
    mBaseShader.compile();
    mPositionLocation = glGetAttribLocation(mBaseShader.getId(), "in_position");
    mColorLocation = glGetAttribLocation(mBaseShader.getId(), "in_color");
    mTexCoordLocation = glGetAttribLocation(mBaseShader.getId(), "in_texCoords");

    Projection proj;
    mProjection = proj.createOrthoProjection(0.0f, (GLfloat)viewSize.x, 0.0f, (GLfloat)viewSize.y, 0.000000001f, 100.0f);
}

void SimpleRendering::loop()
{
    //rendering
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //mVao.bind();
    
    glEnableVertexAttribArray(mPositionLocation);
    glEnableVertexAttribArray(mColorLocation);
    glEnableVertexAttribArray(mTexCoordLocation);

        mTriangles.bind();
        glVertexAttribPointer(mPositionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
        mColors.bind();
        glVertexAttribPointer(mColorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
        mTexCoords.bind();
        glVertexAttribPointer(mTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

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

        glDrawArrays(GL_TRIANGLES, 0, mTriangles.getElementAmount() / 3);
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
