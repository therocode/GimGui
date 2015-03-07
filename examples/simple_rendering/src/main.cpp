#include <gl_core_3_3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "simplerendering.hpp"
#include <memory>
#include <vec2.hpp>

GLFWwindow* window;
const int32_t windowWidth = 800;
const int32_t windowHeight = 600;
SimpleRendering* globalSimple = nullptr;

void key(GLFWwindow* window, int32_t key, int32_t s, int32_t action, int32_t mods)
{
      if(action != GLFW_PRESS) return;

      switch(key) {
          case GLFW_KEY_Z:
              if(mods & GLFW_MOD_SHIFT)
                  ;
              else
                  ;
              break;
          case GLFW_KEY_ESCAPE:
              glfwSetWindowShouldClose(window, GL_TRUE);
              break;
          case GLFW_KEY_UP:
              ;
              break;
          case GLFW_KEY_DOWN:
              ;
              break;
          case GLFW_KEY_LEFT:
              ;
              break;
          case GLFW_KEY_RIGHT:
              ;
              break;
          default:
              return;
      }
}


/* new window size */
void reshape(GLFWwindow* window, int width, int height)
{
    GLfloat h = (GLfloat) height / (GLfloat) width;

    glViewport(0, 0, (GLint) width, (GLint) height);

    if(globalSimple)
        globalSimple->setViewSize(Vec2({width, height}));
}

int main()
{
    auto glStatus = ogl_LoadFunctions();
    if(glStatus == ogl_LOAD_FAILED)
    {
        std::cout << "Could not initialize OpenGL\n";
    }

    if(!glfwInit())
    {
        std::cout << "Could not initialize GLFW\n";
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_DEPTH_BITS, 16);

    window = glfwCreateWindow(800, 600, "GimGui", NULL, NULL);

    if(!window)
    {
        std::cout << "Could not create GLFW window\n";
        glfwTerminate();
        exit(1);
    }

    // Set callback functions
    glfwSetFramebufferSizeCallback(window, reshape);
    glfwSetKeyCallback(window, key);

    glfwMakeContextCurrent(window);
    glfwSwapInterval( 1 );

    int32_t width;
    int32_t height;

    glfwGetFramebufferSize(window, &width, &height);
    reshape(window, width, height);

    std::unique_ptr<SimpleRendering> simple = std::unique_ptr<SimpleRendering>(new SimpleRendering(Vec2({800, 600})));
    globalSimple = &*simple;

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        simple->loop();
        // Swap buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}
