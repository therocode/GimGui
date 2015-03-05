#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    if(!glfwInit())
    {
        std::cout << "Could not initialize GLFW\n";
        exit(1);
    }
}
