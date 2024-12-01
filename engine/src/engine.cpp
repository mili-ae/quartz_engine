#define STB_IMAGE_IMPLEMENTATION

#include "common.hpp"
#include "engine.hpp"
#include "mesh.cpp"
#include "shader.cpp"
#include "texture.cpp"
#include "directional_light.cpp"
#include "point_light.cpp"
#include "spot_light.cpp"
#include "material.cpp"

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
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f);

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

        // glClearColor(128.0f / 255.0f, 218.0f / 255.0f, 243.0f / 255.0f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        func();

        glUseProgram(0);

        window.swapBuffers();
    }
}

