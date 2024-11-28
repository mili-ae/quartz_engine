#pragma once

#include <stdio.h>
#include "../third_party/glew/glew.h"
#include "../third_party/glfw/glfw3.h"
// #include <glew/glew.h>
// #include <glfw/glfw3.h>

class Window
{
    GLFWwindow *window;
    const char* title;
public:
    GLint width, height;
    GLint bufferWidth, bufferHeight;

    Window();
    Window(GLint w, GLint h, const char* t);
    ~Window();

    bool init();
    bool shouldClose() { return glfwWindowShouldClose(window); }
    void swapBuffers() { glfwSwapBuffers(window); }
};