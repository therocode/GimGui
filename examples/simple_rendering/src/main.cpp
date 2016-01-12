#include <iostream>
#include "simplerendering.hpp"
#include <memory>
#include <helpers/vector2.hpp>
#include <window.hpp>

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>

static SimpleRendering* simpleInstance = nullptr;
static Window* windowInstance = nullptr;

void trampoline()
{   
    if(simpleInstance != nullptr && windowInstance != nullptr)
    {   
        if(windowInstance->isOpen())
        {
            simpleInstance->handleEvents(windowInstance->pollEvents());
            simpleInstance->loop();
            windowInstance->swapBuffers();

            if(simpleInstance->isTerminated())
            {
                std::cout << "closing\n";
                windowInstance->close();
            }
        }
        else
        {   
            emscripten_cancel_main_loop();
        }   
    }   
}   
#endif


int main()
{
    Window window({800, 600});
    std::unique_ptr<SimpleRendering> simple = std::unique_ptr<SimpleRendering>(new SimpleRendering(Vector2({800, 600})));

#if defined(__EMSCRIPTEN__)
    simpleInstance = simple.get();
    windowInstance = &window;
    emscripten_set_main_loop(trampoline, 60, true);
#else
    while(window.isOpen())
    {
        simple->handleEvents(window.pollEvents());

        simple->loop();

        if(simple->isTerminated())
            window.close();

        window.swapBuffers();
    }
#endif
}
