#include "quartz_lib.hpp"
#include "window.cpp"

Window window = Window();

EXPORT_FN void init()
{
    if (!window.init())
    {
        printf("Failed to init window");
    }
}

EXPORT_FN void update(void (*func)() = nullptr)
{

    while (!window.shouldClose())
    {
        glfwPollEvents();

        glClearColor(128.0f / 255.0f, 218.0f / 255.0f, 243.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        func();

        glUseProgram(0);

        window.swapBuffers();
    }
}

