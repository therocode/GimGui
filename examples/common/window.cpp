#include <window.hpp>
#include <iostream>

Window::Window(const Vec2& size)
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

    mWindow = glfwCreateWindow(800, 600, "GimGui", NULL, NULL);

    if(!mWindow)
    {
        std::cout << "Could not create GLFW window\n";
        glfwTerminate();
        exit(1);
    }

    glfwSetWindowUserPointer(mWindow, this);

    //Set callback functions
    glfwSetFramebufferSizeCallback(mWindow, reshape);
    glfwSetKeyCallback(mWindow, key);

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval( 1 );

    int32_t width;
    int32_t height;

    glfwGetFramebufferSize(mWindow, &width, &height);
    //reshape(window, width, height);

}

Window::~Window()
{
    if(isOpen())
        close();
}

void Window::key(GLFWwindow* window, int32_t key, int32_t s, int32_t action, int32_t mods)
{
      if(action != GLFW_PRESS) return;

      Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

      thisWindow->mKeyEvents.push_back(key);
}

void Window::reshape(GLFWwindow* window, int width, int height)
{
    GLfloat h = (GLfloat) height / (GLfloat) width;

    glViewport(0, 0, (GLint) width, (GLint) height);

    Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    thisWindow->mResizeEvents.push_back(Vec2({width, height}));
}

void Window::pollEvents()
{
    glfwPollEvents();
}

bool Window::isOpen() const
{
    return mWindow != nullptr;
}

void Window::close()
{
    glfwSetWindowShouldClose(mWindow, GL_TRUE);
    glfwTerminate();
    mWindow = nullptr;
}

void Window::swapBuffers()
{
    glfwSwapBuffers(mWindow);

    mKeyEvents.clear();
    mResizeEvents.clear();
}

Events Window::fetchEvents()
{
    Events toReturn;
    std::swap(toReturn.keyEvents, mKeyEvents);
    std::swap(toReturn.resizeEvents, mResizeEvents);

    return toReturn;
}
