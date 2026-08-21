
#ifndef __ENGINE_DEBUG_HPP__
#define __ENGINE_DEBUG_HPP__
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

class EngineDebug
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;

public:
    EngineDebug(SDL_Window *window, SDL_Renderer *renderer);
    ~EngineDebug();
    void NewFrame();
    void Render(SDL_Renderer *renderer);
    void ProcessEvent(SDL_Event *event);
};

#endif // __ENGINE_DEBUG_HPP__
