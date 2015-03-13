#pragma once
#include <helpers/vec2.hpp>
#include <opengl.hpp>
#include <SDL2/SDL.h>
#include <deque>

class Window
{
    public:
        Window(const Vec2& size);
        ~Window();
        Window(const Window& other) = delete;
        Window& operator=(const Window& other) = delete;
        std::deque<SDL_Event> pollEvents();
        bool isOpen() const;
        void close();
        void swapBuffers();
    private:
        SDL_Window* mWindow;
        SDL_GLContext mGlContext;
};
