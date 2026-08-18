#ifndef __IMGUI_BASIC_HPP__
#define __IMGUI_BASIC_HPP__
#include <SDL3/SDL.h>

#include <imgui.h> 
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>


struct BasicImgui {
    BasicImgui(SDL_Window* window, SDL_Renderer* renderer);
    ~BasicImgui();
    void ProcessEvent(SDL_Event* event);
    void Render(SDL_Renderer *renderer);
    void NewFrame();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool show_dialog;
    
    void UpdateDisplaySize();
};

#endif // __IMGUI_BASIC_HPP__