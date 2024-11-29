#include "engine.hpp"

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
        // Calculate deltaTime
        GLfloat current = glfwGetTime();
        deltaTime = current - lastTime;
        lastTime = current;
        float fps = 1.0f / deltaTime;
        // printf("FPS: %.4f\n", fps);

        glfwPollEvents();

        camera.keyControl(window.getKeys(), deltaTime);
        camera.mouseControl(window.getXChange(), window.getYChange());

        glClearColor(128.0f / 255.0f, 218.0f / 255.0f, 243.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        func();

        glUseProgram(0);

        window.swapBuffers();
    }
}

