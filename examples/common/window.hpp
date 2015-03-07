#pragma once
#include <vec2.hpp>
#include <gl_core_3_3.h>
#include <GLFW/glfw3.h>
#include <deque>

struct Events
{
    std::deque<Vec2> resizeEvents;
    std::deque<int32_t> keyEvents;
};

class Window
{
    public:
        Window(const Vec2& size);
        ~Window();
        Window(const Window& other) = delete;
        Window& operator=(const Window& other) = delete;
        static void key(GLFWwindow* window, int32_t key, int32_t s, int32_t action, int32_t mods);
        static void reshape(GLFWwindow* window, int width, int height);
        void pollEvents();
        bool isOpen() const;
        void close();
        void swapBuffers();
        Events fetchEvents();
    private:
        GLFWwindow* mWindow;
        std::deque<Vec2> mResizeEvents;
        std::deque<int32_t> mKeyEvents;
};
//void key(GLFWwindow* window, int32_t key, int32_t s, int32_t action, int32_t mods)
//{
//      if(action != GLFW_PRESS) return;
//
//      switch(key) {
//          case GLFW_KEY_Z:
//              if(mods & GLFW_MOD_SHIFT)
//                  ;
//              else
//                  ;
//              break;
//          case GLFW_KEY_ESCAPE:
//              break;
//          case GLFW_KEY_UP:
//              ;
//              break;
//          case GLFW_KEY_DOWN:
//              ;
//              break;
//          case GLFW_KEY_LEFT:
//              ;
//              break;
//          case GLFW_KEY_RIGHT:
//              ;
//              break;
//          default:
//              return;
//      }
//}
//
//

/* new window size */
//void reshape(GLFWwindow* window, int width, int height)
//{
//    GLfloat h = (GLfloat) height / (GLfloat) width;
//
//    glViewport(0, 0, (GLint) width, (GLint) height);
//
//    if(globalSimple)
//        globalSimple->setViewSize(Vec2({width, height}));
//}
