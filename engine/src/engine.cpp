#include "quartz_lib.hpp"
#include "window.cpp"

EXPORT_FN void update()
{
    Window window = Window();
    if (!window.init())
    {
        printf("Failed to init window");
    }

    while (!window.shouldClose())
    {
        glfwPollEvents();

        glClearColor(128.0f / 255.0f, 218.0f / 255.0f, 243.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.swapBuffers();
    }
}