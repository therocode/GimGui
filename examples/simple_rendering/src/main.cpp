#include <iostream>
#include "simplerendering.hpp"
#include <memory>
#include <helpers/vec2.hpp>
#include <window.hpp>

int main()
{
    Window window({800, 600});
    std::unique_ptr<SimpleRendering> simple = std::unique_ptr<SimpleRendering>(new SimpleRendering(Vec2({800, 600})));

    while(window.isOpen())
    {
        simple->handleEvents(window.pollEvents());

        simple->loop();

        if(simple->isTerminated())
            window.close();

        window.swapBuffers();
    }
}
