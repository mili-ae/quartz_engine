#include "window_sdl.hpp"

WindowSDL::WindowSDL()
{
    width = 1366;
    height = 768;
    title = "Quartz Engine";
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
    isRunning = true;
}

WindowSDL::~WindowSDL()
{
}

SDL_AppResult WindowSDL::SDL_AppInit()
{
    SDL_SetAppMetadata(title, "1.0", "com.nicocchi.quartz-engine");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(title, width, height, 0, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

void WindowSDL::SDL_PollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_EVENT_QUIT:
            {
                isRunning = false;
            } break;
            case SDL_EVENT_KEY_DOWN:
            {
                if (event.key.key == SDLK_ESCAPE) {
                    isRunning = false;
                }
            } break;
        }
    }
}

SDL_AppResult WindowSDL::SDL_AppIterate()
{
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
    /* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
    const float red = (float) (0.5 + 0.5 * SDL_sin(now));
    const float green = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
    const float blue = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
    SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}