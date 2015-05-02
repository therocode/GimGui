#include "simplerendering.hpp"
#include <glutils/baseshader.hpp>
#include <glutils/uniform.hpp>
#include <textureadaptor.hpp>
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
#include <gimgui/data/font.hpp>

Callback addText = [] (gim::Element& self, const Parameters& parameters)
{
    const std::string source = u8"this is a space: and this is a tab\tand this is a newline\ndone";

    std::string text = self.getAttribute<std::string>("text");
    text.push_back(source[text.size()]);
    self.setAttribute("text", text);
};

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
        newSize.y = size.y + delta.y;
        newSize.x = newSize.y;

        float newTextSize = newSize.y / 256.0f;
        self.setAttribute("text_scale", newTextSize);

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
    mFontFile("resources/fonts/LiberationSans-Regular.ttf", std::ios::binary),
    mBoldFontFile("resources/fonts/LiberationSans-Bold.ttf", std::ios::binary),
    mItalicFontFile("resources/fonts/LiberationSans-Italic.ttf", std::ios::binary),
    mBoldItalicFontFile("resources/fonts/LiberationSans-BoldItalic.ttf", std::ios::binary),
    mFont(mFontFile),
    mBoldFont(mBoldFontFile),
    mItalicFont(mItalicFontFile),
    mBoldItalicFont(mBoldItalicFontFile),
    mRoot({"container"},
        {
            {"name" , std::string("red")},
            {"color",    Color(140, 35, 24)},
            {"original_color",    Color(140, 35, 24)},
            {"click_color",    Color(174, 70, 56)},
            {"position", Vec2({0, 0})},
            {"size",     Vec2({256, 256})},
            {"stretch_mode", gim::StretchMode::STRETCHED},
            {"image_id", 0u},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({8, 8}), Vec2({48, 48}))},
            {"dragged", 0},
            {"resize", false},
            {"on_click", CallbackList({setClickColor, printClicked, setResize, addText})},
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
            {"border_coords_l",  gim::Rectangle<Vec2>(Vec2({0 ,8 }), Vec2({8 ,48}))},
            {"text", std::string(u8"")},
            {"text_size", 30},
            {"text_scale", 1.0f},
            {"font", gim::makeRef(mFont)},
            {"bold_font", gim::makeRef(mBoldFont)},
            {"italic_font", gim::makeRef(mItalicFont)},
            {"bold_italic_font", gim::makeRef(mBoldItalicFont)},
            {"text_color", Color(0, 20, 200, 150)},
            {"character_spacing", 0.0f},
            {"line_spacing", 0.0f},
            {"tab_width", 4},
            {"text_style", gim::TextStyle::BOLD | gim::TextStyle::ITALIC},
            //{"line_wrap", gim::Wrap::Characters},
            {"text_borders", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({250, 250}))},
            /*{"text_style", gim::NORMAL | gim::BOLD | gim::UNDERLINED | gim::ITALIC | gim::STRIKETHROUGH | gim::HOLLOW},
            {"text_bg_color", Color(100, 200, 20, 12)},
            {"text_alignment, [gim::LEFT_ALIGN|gim::RIGHT_ALIGN|gim::CENTER_ALIGN|gim::JUSTIFY_ALIGN]
            {"text_avoid_blocks", std::vector<Rectanglee>()},
            {"markup", true"}
            */
        },
        {
            gim::Element({"child"},
            {
                {"name" , std::string("blue")},
                {"color",    Color(94, 140, 106, 100)},
                {"original_color",    Color(94, 140, 106, 100)},
                {"click_color",    Color(124, 170, 136)},
                {"position", Vec2({20, 20})},
                {"size",     Vec2({56, 18})},
                {"stretch_mode", gim::StretchMode::STRETCHED},
                {"image_id", 0u},
                {"image_coords", gim::Rectangle<Vec2>(Vec2({8, 8}), Vec2({48, 48}))},
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
                {"color",    Color(136, 166, 94, 20)},
                {"original_color",    Color(136, 166, 94, 20)},
                {"click_color",    Color(166, 196, 124)},
                {"position", Vec2({90, 20})},
                {"size",     Vec2({64, 64})},
                {"stretch_mode", gim::StretchMode::STRETCHED},
                {"image_id", 0u},
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
                    {"image_id", 0u},
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
    std::unique_ptr<Texture> fontTexture = std::unique_ptr<Texture>(new Texture());
    auto textureId = mRenderDataGenerator.registerFontStorage({mFont, mBoldFont, mItalicFont, mBoldItalicFont}, TextureAdaptor(*fontTexture));
    mTextures.emplace(textureId, std::move(fontTexture));

    //load textures
    uint32_t textureIdA = mRenderDataGenerator.registerImageInfo({64, 64});
    uint32_t textureIdB = mRenderDataGenerator.registerImageInfo({56, 18});

    mTextures.emplace(textureIdA, std::unique_ptr<Texture>(new Texture(loadTexture("resources/borders.png"))));
    mTextures.emplace(textureIdB, std::unique_ptr<Texture>(new Texture(loadTexture("resources/button.png"))));

    mRoot.setAttribute("image_id", textureIdA);
    for(auto& child : mRoot.children())
        child->setAttribute("image_id", textureIdA);

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
         1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  0.0f, 1.0f, 1.0f,
         0.0f,  1.0f, 1.0f, 1.0f 
    }));

    //mVao.setVertexAttribute(0, 3, mTriangles);
    //mVao.setVertexAttribute(1, 4, mColors);
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
        glVertexAttribPointer(mColorLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
        mTexCoords.bind();
        glVertexAttribPointer(mTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

    mBaseShader.activate();
    mBaseShader.setUniform("projection", UniformType::MAT4X4, &mProjection[0]);

    auto renderDatas = mRenderDataGenerator.generate(mRoot);


    for(auto renderData : renderDatas)
    {
        if(renderData.positions.size() > 0)
        {
            GIM_ASSERT(mTextures.count(renderData.imageId) != 0, "invalid texture given");
            mBaseShader.setUniform("texture", UniformType::TEXTURE, &*mTextures.at(renderData.imageId));
            mTriangles.setData(renderData.positions);
            mColors.setData(renderData.colors);

            mTexCoords.setData(renderData.texCoords);

            glDrawArrays(GL_TRIANGLES, 0, mTriangles.getElementAmount() / 3);
        }

        if(renderData.textPositions.size() > 0)
        {
            GIM_ASSERT(mTextures.count(renderData.textImageId) != 0, "invalid texture given");
            mBaseShader.setUniform("texture", UniformType::TEXTURE, &*mTextures.at(renderData.textImageId));
            mTriangles.setData(renderData.textPositions);
            mColors.setData(renderData.textColors);
            mTexCoords.setData(renderData.textTexCoords);

            glDrawArrays(GL_TRIANGLES, 0, mTriangles.getElementAmount() / 3);
        }
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
