#include "window.hpp"

Window::Window()
{
    width = 800;
    height = 600;
    title = "Quartz Engine";
}

Window::Window(GLint w, GLint h, const char* t) : width(w), height(h), title(t)
{
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::init()
{
    // Init GLFW
    if (!glfwInit())
    {
        printf("GLFW Initialization failed");
        glfwTerminate();
        return false;
    }

    // GLFW Window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        printf("GLFW window creation failed");
        glfwTerminate();
        return false;
    }

    // Get buffer size info
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Set context for GLEW
    glfwMakeContextCurrent(window);

    // Allow modern extensions
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW Initialization failed");
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    return true;
}