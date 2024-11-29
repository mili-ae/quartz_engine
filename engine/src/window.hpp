#pragma once

#include <stdio.h>
#include "../third_party/glew/glew.h"
#include "../third_party/glfw/glfw3.h"

class Window
{
    GLFWwindow *window;
    const char* title;

    bool keys[1024];
    GLfloat lastX;
    GLfloat lastY;

    // How much it has changed since last movement (lastX/Y)
    GLfloat xChange;
    GLfloat yChange;

    bool mouseFirstMoved;

    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
    void createCallbacks();

public:
    GLint width, height;
    GLint bufferWidth, bufferHeight;

    Window();
    Window(GLint w, GLint h, const char* t);
    ~Window();

    bool init();
    bool shouldClose() { return glfwWindowShouldClose(window); }
    void swapBuffers() { glfwSwapBuffers(window); }

    bool* getKeys() { return keys; }
    GLfloat getXChange();
    GLfloat getYChange();
};