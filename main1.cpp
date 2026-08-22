#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "const.hpp"
#include "customtext.hpp"
#include "sprite.hpp"
#include "basicimgui.hpp"
#include <flecs.h>
#include "controls.hpp"
struct SDLApplication
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    flecs::world world;
    // entt::registry registry;

    // SDL_Surface *surface;
    int r = 200;
    int g = 100;
    int b = 50;
    bool running = true;
    bool fullscreen = false;

    TextElement *text;
    BasicImgui *imgui;

    b2WorldId physicsWorld;

    SDLApplication(const char *title)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed: %s", SDL_GetError());
        }
        window = SDL_CreateWindow(title, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
        renderer = SDL_CreateRenderer(window, nullptr);
        // renderer = SDL_CreateRenderer(window, "vulkan");

        // Removed letterbox mode to fix ImGui mouse coordinate mismatch on resize
        // SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

        if (window == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow failed: %s", SDL_GetError());
        }

        SDL_Log(std::format("Renderer: {}", SDL_GetRendererName(renderer)).c_str());
        for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i)
        {
            SDL_Log(std::format("Render Driver {}: {}", i, SDL_GetRenderDriver(i)).c_str());
        }
        if (!TTF_Init())
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_Init failed: %s", SDL_GetError());
        }

        CreateText(renderer, world, "Hello SDL3!");

        imgui = new BasicImgui(window, renderer);

        // box2d world initialization
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = {0.0f, 9.8f};
        physicsWorld = b2CreateWorld(&worldDef);

        // ground start - full width ground at bottom of screen
        b2BodyDef groundBodyDef = b2DefaultBodyDef();

        groundBodyDef.position = {
            PixelsToMeters(WINDOW_WIDTH / 2.0f),
            PixelsToMeters(WINDOW_HEIGHT - 30.0f / 2.0f)};

        b2BodyId groundBody = b2CreateBody(
            physicsWorld,
            &groundBodyDef);

        b2Polygon groundBox = b2MakeBox(
            PixelsToMeters(WINDOW_WIDTH / 2.0f),
            PixelsToMeters(30.0f / 2.0f));

        b2ShapeDef groundShapeDef = b2DefaultShapeDef();

        b2CreatePolygonShape(
            groundBody,
            &groundShapeDef,
            &groundBox);

        // player start
        b2BodyDef playerBodyDef = b2DefaultBodyDef();

        playerBodyDef.type = b2_dynamicBody;

        playerBodyDef.position = {
            PixelsToMeters(WINDOW_WIDTH / 2.0f),
            PixelsToMeters(WINDOW_HEIGHT / 2.0f)};

        b2BodyId playerBody = b2CreateBody(
            physicsWorld,
            &playerBodyDef);

        // give shape to player body
        b2Polygon playerBox = b2MakeBox(
            PixelsToMeters(32.0f),
            PixelsToMeters(50.0f));

        b2ShapeDef playerShapeDef = b2DefaultShapeDef();

        playerShapeDef.density = 1.0f;
        playerShapeDef.material.friction = 0.5f;

        b2CreatePolygonShape(
            playerBody,
            &playerShapeDef,
            &playerBox);

        CreateSprite(renderer, world, "../../assets/images/player.png", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, true, playerBody);
        CreateSprite(renderer, world, "../../assets/images/ground.png", WINDOW_WIDTH / 2, WINDOW_HEIGHT - 30, false, groundBody);
    }

    ~SDLApplication()
    {
        if (imgui)
            delete imgui;

        TTF_Quit();
        DestroyText(world);
        DestroySprites(world);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void Tick()
    {
        Input();
        Update();
        Render();
    }
    void Input()
    {
        const bool *keys = SDL_GetKeyboardState(nullptr);
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // Pass event to ImGui
            imgui->ProcessEvent(&event);
            HandelEvent(&event, r, g, b, running, fullscreen, window);
        }

        if (keys[SDL_SCANCODE_L] == true)
        {
            SDL_Log("Key pressed: %s", SDL_GetKeyName(SDLK_ESCAPE));
        }

        float x, y;
        SDL_MouseButtonFlags state = SDL_GetMouseState(&x, &y);
        // if (state & SDL_BUTTON_LEFT)
        // {
        //     SDL_Log("Mouse position: (%f, %f)", x, y);
        // }
    }

    void Update()
    {
        // Update game logic here
    }

    void Render()
    {

        b2World_Step(
            physicsWorld,
            1.0f / 60.0f,
            4);

        // --------------------------------
        // Sync physics -> ECS
        // --------------------------------

        SyncPhysicsToTransform(world);

        // Start ImGui frame
        imgui->NewFrame();

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderClear(renderer);

        RenderSprites(renderer, world);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(renderer, 10, 10, "Hello SDL3!");
        RenderText(renderer, world);

        imgui->Render(renderer, world);

        SDL_RenderPresent(renderer);
    }

    void MainLoop()
    {
        Uint64 fps = 0;
        Uint64 fpsTimer = SDL_GetTicks();

        while (running)
        {
            Tick();

            fps++;

            Uint64 now = SDL_GetTicks();

            if (now - fpsTimer >= 1000)
            {
                // SDL_Log("FPS: %llu", fps);
                SDL_SetWindowTitle(window, std::format("Vulkan Window - FPS: {}", fps).c_str());

                fps = 0;
                fpsTimer = now;
            }
        }
    }
};

int main()
{

    SDLApplication app("Vulkan Window");
    app.MainLoop();
    return EXIT_SUCCESS;
}