#include "simplerendering.hpp"
#include <gl_core_3_3.h>

void SimpleRendering::loop()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
