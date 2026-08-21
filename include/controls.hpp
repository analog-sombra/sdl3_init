#ifndef __CONTROLS_HPP__
#define __CONTROLS_HPP__

#include <SDL3/SDL.h>
void HandelEvent(SDL_Event *event, int &r, int &g, int &b, bool &running, bool &fullscreen, SDL_Window *window);

#endif // __CONTROLS_HPP__