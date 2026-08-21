#include "controls.hpp"

void HandelEvent(SDL_Event *event, int &r, int &g, int &b, bool &running, bool &fullscreen, SDL_Window *window)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        running = false;
    }
    else if (event->type == SDL_EVENT_KEY_DOWN)
    {
        SDL_Log("Key pressed: %s", SDL_GetKeyName(event->key.key));
        // SDL_Log("Key pressed: %d", event.key.key);

        if (event->key.key == SDLK_1)
            r = 150, b = 50, g = 50;
        if (event->key.key == SDLK_2)
            r = 50, b = 150, g = 50;
        if (event->key.key == SDLK_3)
            r = 50, b = 50, g = 150;
        if (event->key.key == SDLK_0)
            r = 200, b = 100, g = 50;
        if (event->key.key == SDLK_F11)
        {
            SDL_SetWindowFullscreen(window, fullscreen);
            fullscreen = !fullscreen;
        }
    }
    else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
            SDL_Log("Mouse button pressed: Left");
        else if (event->button.button == SDL_BUTTON_RIGHT)
            SDL_Log("Mouse button pressed: Right");
        else if (event->button.button == SDL_BUTTON_MIDDLE)
            SDL_Log("Mouse button pressed: Middle");
        else
            SDL_Log("Mouse button pressed: %d", event->button.clicks);
    }
}