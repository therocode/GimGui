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
#include <gimgui/util/getorfallback.hpp>
#include <gimgui/util/resolve.hpp>

Callback moveOrResize = [] (gim::Element& self, const Parameters& parameters)
{
    Vec2 position = getOrFallback(parameters, "position", Vec2({0,0}));
    Vec2 delta = getOrFallback(parameters, "delta", Vec2({0,0}));

    if(!getOrFallback(self, "resize", false))
    {
        Vec2 relativePosition = self.getAttribute<Vec2>("position");
        Vec2 newPosition;
        newPosition.x = relativePosition.x + delta.x;
        newPosition.y = relativePosition.y + delta.y;

        self.setAttribute("position", newPosition);
    }
    else
    {
        Vec2 size = self.getAttribute<Vec2>("size");
        Vec2 newSize;
        newSize.x = size.x + delta.x;
        newSize.y = size.y + delta.y;

        self.setAttribute("size", newSize);
    }
};

Callback setResize = [] (gim::Element& self, const Parameters& parameters)
{
    Vec2 clickPos = getOrFallback(parameters, "position", Vec2({0,0}));
    gim::AbsoluteMap<Vec2> absoluteMap("position");
    const Vec2 position = absoluteMap.getAbsoluteOf(self);

    Vec2 relativeClickPos;
    relativeClickPos.x = clickPos.x - position.x;
    relativeClickPos.y = clickPos.y - position.y;
    const Vec2 size = self.getAttribute<Vec2>("size");

    if(relativeClickPos.x > size.x - 20 && relativeClickPos.y > size.y - 20)
    {
        self.setAttribute("resize", true);
    }
};

Callback unsetResize = [] (gim::Element& self, const Parameters& parameters)
{
    self.setAttribute("resize", false);
};

Callback setClickColor = [] (gim::Element& self, const Parameters& parameters)
{
    self.setAttribute("color", self.getAttribute<Color>("click_color"));
};
Callback setOriginalColor = [] (gim::Element& self, const Parameters& parameters)
{
    int32_t releasedButton = getOrFallback(parameters, "button", UNKNOWN);
    int32_t draggedButton = self.getAttribute<int32_t>("dragged");
    if(draggedButton == releasedButton && draggedButton != 0)
    {
        self.setAttribute("color", self.getAttribute<Color>("original_color"));
    }
};
Callback printClicked = [] (gim::Element& self, const Parameters& parameters)
{
    std::cout << self.getAttribute<std::string>("name") << " clicked\n";
};
Callback toggleMode = [] (gim::Element& self, const Parameters& parameters)
{
    int32_t releasedButton = getOrFallback(parameters, "button", UNKNOWN);
    if(!self.getAttribute<bool>("resize"))
    {
        if(releasedButton == RIGHT)
        {
            gim::StretchMode currentStretchMode = self.getAttribute<gim::StretchMode>("stretch_mode");
            gim::StretchMode newStretchMode;

            newStretchMode = gim::resolve<gim::StretchMode>(currentStretchMode, {{gim::StretchMode::STRETCHED, gim::StretchMode::TILED},
                                                          {gim::StretchMode::TILED, gim::StretchMode::V_TILED},
                                                          {gim::StretchMode::V_TILED, gim::StretchMode::H_TILED},
                                                          {gim::StretchMode::H_TILED, gim::StretchMode::STRETCHED}});

            self.setAttribute("stretch_mode", newStretchMode);
        }
        else if(releasedButton == MIDDLE)
        {
            gim::BorderMode currentBorderMode = self.getAttribute<gim::BorderMode>("border_mode");
            gim::BorderMode newBorderMode;

            newBorderMode = gim::resolve<gim::BorderMode>(currentBorderMode, {{gim::BorderMode::NONE, gim::BorderMode::LEFT_RIGHT},
                                                          {gim::BorderMode::LEFT_RIGHT, gim::BorderMode::TOP_BOTTOM},
                                                          {gim::BorderMode::TOP_BOTTOM, gim::BorderMode::FULL},
                                                          {gim::BorderMode::FULL, gim::BorderMode::NONE}});

            self.setAttribute("border_mode", newBorderMode);
        }
    }
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
            {"image_coords", gim::Rectangle<Vec2>(Vec2({8, 8}), Vec2({48, 48}))},
            {"dragged", 0},
            {"resize", false},
            {"on_click", CallbackList({setClickColor, printClicked, setResize})},
            {"on_release", CallbackList({toggleMode})},
            {"on_global_release", CallbackList({setOriginalColor, unsetResize})},
            {"on_drag", moveOrResize},
            {"border_mode", gim::BorderMode::FULL},
            {"border_coords_tl", gim::Rectangle<Vec2>(Vec2({0 ,0 }), Vec2({8 ,8 }))},
            {"border_coords_t",  gim::Rectangle<Vec2>(Vec2({8 ,0 }), Vec2({48,8 }))},
            {"border_coords_tr", gim::Rectangle<Vec2>(Vec2({56,0 }), Vec2({8 ,8 }))},
            {"border_coords_r",  gim::Rectangle<Vec2>(Vec2({56,8 }), Vec2({8 ,48}))},
            {"border_coords_br", gim::Rectangle<Vec2>(Vec2({56,56}), Vec2({8 ,8 }))},
            {"border_coords_b",  gim::Rectangle<Vec2>(Vec2({8 ,56}), Vec2({48,8 }))},
            {"border_coords_bl", gim::Rectangle<Vec2>(Vec2({0 ,56}), Vec2({8 ,8 }))},
            {"border_coords_l",  gim::Rectangle<Vec2>(Vec2({0 ,8 }), Vec2({8 ,48}))}/*,
            {"text", "I am the text of this element"},
            {"text_offset", Vec2({10, 10})},
            {"text_size", 23},
            {"text_color", Color(255, 100, 0)},
            {"text_style", gim::Bold | gim::Underlined},
            {"text_style", gim::Bold | gim::Underlined},
            {"text_scale", 1.0f},
            {"text_font", 0},
            {"text_wrap", 100},

            

            class TextureBrigde
            {
                int32_t createTexture(size);
                void updateRegion(id, start, size, bytes);
                bool enlarge(id, newsize);
            };

            */
        },
        {
            gim::Element({"child"},
            {
                {"name" , std::string("blue")},
                {"color",    Color(94, 140, 106)},
                {"original_color",    Color(94, 140, 106)},
                {"click_color",    Color(124, 170, 136)},
                {"position", Vec2({20, 20})},
                {"size",     Vec2({56, 18})},
                {"stretch_mode", gim::StretchMode::STRETCHED},
                {"image_id", 1},
                {"image_coords", gim::Rectangle<Vec2>(Vec2({6, 0}), Vec2({44, 18}))},
                {"block_event", true},
                {"dragged", 0},
                {"resize", false},
                {"on_click", CallbackList({setClickColor, printClicked, setResize})},
                {"on_release", CallbackList({toggleMode})},
                {"on_global_release", CallbackList({setOriginalColor, unsetResize})},
                {"on_drag", moveOrResize},
                {"border_mode", gim::BorderMode::LEFT_RIGHT},
                {"border_coords_tl", gim::Rectangle<Vec2>(Vec2({0 ,0 }), Vec2({8 ,8 }))},
                {"border_coords_t",  gim::Rectangle<Vec2>(Vec2({8 ,0 }), Vec2({48,8 }))},
                {"border_coords_tr", gim::Rectangle<Vec2>(Vec2({56,0 }), Vec2({8 ,8 }))},
                {"border_coords_r",  gim::Rectangle<Vec2>(Vec2({56,8 }), Vec2({8 ,48}))},
                {"border_coords_br", gim::Rectangle<Vec2>(Vec2({56,56}), Vec2({8 ,8 }))},
                {"border_coords_b",  gim::Rectangle<Vec2>(Vec2({8 ,56}), Vec2({48,8 }))},
                {"border_coords_bl", gim::Rectangle<Vec2>(Vec2({0 ,56}), Vec2({8 ,8 }))},
                {"border_coords_l",  gim::Rectangle<Vec2>(Vec2({0 ,8 }), Vec2({8 ,48}))}
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
                {"image_coords", gim::Rectangle<Vec2>(Vec2({8, 8}), Vec2({48, 48}))},
                {"dragged", 0},
                {"resize", false},
                {"on_click", CallbackList({setClickColor, printClicked, setResize})},
                {"on_release", CallbackList({toggleMode})},
                {"on_global_release", CallbackList({setOriginalColor, unsetResize})},
                {"block_event", true},
                {"on_drag", moveOrResize},
                {"border_mode", gim::BorderMode::LEFT_RIGHT},
                {"border_coords_tl", gim::Rectangle<Vec2>(Vec2({0 ,0 }), Vec2({8 ,8 }))},
                {"border_coords_t",  gim::Rectangle<Vec2>(Vec2({8 ,0 }), Vec2({48,8 }))},
                {"border_coords_tr", gim::Rectangle<Vec2>(Vec2({56,0 }), Vec2({8 ,8 }))},
                {"border_coords_r",  gim::Rectangle<Vec2>(Vec2({56,8 }), Vec2({8 ,48}))},
                {"border_coords_br", gim::Rectangle<Vec2>(Vec2({56,56}), Vec2({8 ,8 }))},
                {"border_coords_b",  gim::Rectangle<Vec2>(Vec2({8 ,56}), Vec2({48,8 }))},
                {"border_coords_bl", gim::Rectangle<Vec2>(Vec2({0 ,56}), Vec2({8 ,8 }))},
                {"border_coords_l",  gim::Rectangle<Vec2>(Vec2({0 ,8 }), Vec2({8 ,48}))}
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
                    {"dragged", 0},
                    {"resize", false},
                    {"image_coords", gim::Rectangle<Vec2>(Vec2({8, 8}), Vec2({48, 48}))},
                    {"on_click", CallbackList({setClickColor, printClicked, setResize})},
                    {"on_release", CallbackList({toggleMode})},
                    {"on_global_release", CallbackList({setOriginalColor, unsetResize})},
                    {"block_event", true},
                    {"on_drag", moveOrResize},
                    {"border_mode", gim::BorderMode::NONE},
                    {"border_coords_tl", gim::Rectangle<Vec2>(Vec2({0 ,0 }), Vec2({8 ,8 }))},
                    {"border_coords_t",  gim::Rectangle<Vec2>(Vec2({8 ,0 }), Vec2({48,8 }))},
                    {"border_coords_tr", gim::Rectangle<Vec2>(Vec2({56,0 }), Vec2({8 ,8 }))},
                    {"border_coords_r",  gim::Rectangle<Vec2>(Vec2({56,8 }), Vec2({8 ,48}))},
                    {"border_coords_br", gim::Rectangle<Vec2>(Vec2({56,56}), Vec2({8 ,8 }))},
                    {"border_coords_b",  gim::Rectangle<Vec2>(Vec2({8 ,56}), Vec2({48,8 }))},
                    {"border_coords_bl", gim::Rectangle<Vec2>(Vec2({0 ,56}), Vec2({8 ,8 }))},
                    {"border_coords_l",  gim::Rectangle<Vec2>(Vec2({0 ,8 }), Vec2({8 ,48}))}
            })
        })
{
    //load textures

    mTextures.emplace(0, loadTexture("resources/borders.png"));
    mTextures.emplace(1, loadTexture("resources/button.png"));

    mRenderDataGenerator.registerImageInfo(0, {64, 64});
    mRenderDataGenerator.registerImageInfo(1, {56, 18});
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

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
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
            uint8_t button = event.button.button;

            MouseButton converted = gim::resolve(button, {{SDL_BUTTON_LEFT, LEFT}, {SDL_BUTTON_RIGHT, RIGHT}, {SDL_BUTTON_MIDDLE, MIDDLE}}, UNKNOWN);
            mouseClicked(mRoot, position, converted);
        }
        else if(event.type == SDL_MOUSEBUTTONUP)
        {
            Vec2 position = Vec2({event.button.x, event.button.y});
            uint8_t button = event.button.button;

            MouseButton converted = gim::resolve(button, {{SDL_BUTTON_LEFT, LEFT}, {SDL_BUTTON_RIGHT, RIGHT}, {SDL_BUTTON_MIDDLE, MIDDLE}}, UNKNOWN);
            mouseReleased(mRoot, position, converted);
        }
    }
}

void SimpleRendering::quit()
{
    mQuit = true;
}
