#include <window.hpp>
#include <iostream>

Window::Window(const Vector2& size)
{
#if !defined(__EMSCRIPTEN__)
    auto glStatus = ogl_LoadFunctions();
    if(glStatus == ogl_LOAD_FAILED)
    {
        std::cout << "Could not initialize OpenGL\n";
    }
#endif

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Could not initialize SDL2: " + std::string(SDL_GetError()) + "\n";
        exit(1);
    }

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    mWindow = SDL_CreateWindow("gimgui",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            size.x, size.y,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    mGlContext = SDL_GL_CreateContext(mWindow);

    if(mGlContext == nullptr)
    {
        std::cout << "Could not create gl context: " + std::string(SDL_GetError()) + "\n";
        SDL_Quit();
        exit(1);
    }

    if(mWindow == nullptr)
    {
        std::cout << "Could not create SDL2 window: " + std::string(SDL_GetError()) + "\n";
        SDL_Quit();
        exit(1);
    }

    SDL_GL_SetSwapInterval(1); //VSYNC
}

Window::~Window()
{
    if(isOpen())
        close();
}

std::deque<SDL_Event> Window::pollEvents()
{
    SDL_Event event;
    std::deque<SDL_Event> result;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            close();
        }
        else
        {
            result.push_back(event);
        }
    }

    return result;
}

bool Window::isOpen() const
{
    return mWindow != nullptr;
}

void Window::close()
{
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
    mWindow = nullptr;
}

void Window::swapBuffers()
{
    SDL_GL_SwapWindow(mWindow);
}
