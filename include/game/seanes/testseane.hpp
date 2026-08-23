#ifndef __TEST_SEANE_HPP__
#define __TEST_SEANE_HPP__

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <flecs.h>
#include <string>
#include <map>
#include <memory>
#include "engine/element/component.hpp"
#include "engine/assets_manager.hpp"
#include "engine/element/text_element.hpp"
#include "engine/element/image_element.hpp"
#include "engine/seane/seane.hpp"
#include "engine/element/rect_element.hpp"
class TestSeane : public Seane
{
    flecs::system inputSystem;
    flecs::query<TextElement> textQuery;
    flecs::query<ImageElement> imageQuery;

public:
    TestSeane(AssetsManager *assetsManager, SDL_Renderer *renderer);
    ~TestSeane();
    void Render();
    void Update();
    void HandleEvents(SDL_Event *event);
};

#endif // __TEST_SEANE_HPP__