#pragma once

#include <stdio.h>
#include "../third_party/glew/glew.h"
#define SDL_MAIN_HANDLED
#include "../third_party/SDL3/SDL.h"

class WindowSDL {
public:
    GLint width, height;
    GLint bufferWidth, bufferHeight;
    bool keys[1024];
    bool isRunning;

    WindowSDL();
    ~WindowSDL();

    SDL_AppResult SDL_AppInit();
    void SDL_PollEvents();
    void SDL_AppQuit(void *appstate, SDL_AppResult result);
    SDL_AppResult SDL_AppIterate();


private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    const char *title;
};