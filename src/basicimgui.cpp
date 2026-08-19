#include "basicimgui.hpp"

BasicImgui::BasicImgui(SDL_Window *window, SDL_Renderer *renderer)
    : window(window), renderer(renderer), show_dialog(true)
{

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    ImGui::StyleColorsDark();
    // Setup ImGui backends for SDL3 and SDL Renderer
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // Set initial display size
    UpdateDisplaySize();
}

BasicImgui::~BasicImgui()
{
    // Cleanup ImGui
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void BasicImgui::UpdateDisplaySize()
{
    ImGuiIO &io = ImGui::GetIO();
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    io.DisplaySize = ImVec2((float)w, (float)h);
}

void BasicImgui::ProcessEvent(SDL_Event *event)
{
    ImGui_ImplSDL3_ProcessEvent(event);

    // Handle window resize
    if (event->type == SDL_EVENT_WINDOW_RESIZED)
    {
        UpdateDisplaySize();
    }
}

void BasicImgui::NewFrame()
{
    // Update display size every frame to ensure accuracy
    UpdateDisplaySize();

    // Start ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void BasicImgui::Render(SDL_Renderer *renderer, entt::registry &registry)
{

    static int x = 0, y = 0;

    auto playerView = registry.view<Transform, Player>();

    // for (auto entity : player)
    // {
    //     auto &transform = player.get<Transform>(entity);
    //     x = (int)transform.rect.x;
    //     y = (int)transform.rect.y;
    // }

    ImGui::Begin("Hello");

    ImGui::Text("Position");
    ImGui::Text("X: ");
    ImGui::SameLine();
    if (ImGui::SliderInt("X", &x, 0, WINDOW_WIDTH))
    {
        for (auto entity : playerView)
        {
            auto &transform = playerView.get<Transform>(entity);
            transform.rect.x = x;
        }
    }
    ImGui::Text("Y: ");
    ImGui::SameLine();
    if (ImGui::SliderInt("Y", &y, 0, WINDOW_HEIGHT))
    {
        for (auto entity : playerView)
        {
            auto &transform = playerView.get<Transform>(entity);
            transform.rect.y = y;
        }
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 250), ImGuiCond_FirstUseEver);
    // Create a basic dialog box
    if (show_dialog)
    {

        if (ImGui::Begin("Hello ImGui", &show_dialog))
        {
            ImGui::Text("This is a basic ImGui dialog box!");
            ImGui::Separator();

            ImGui::Text("You can click this button:");
            if (ImGui::Button("Click Me!", ImVec2(120, 0)))
            {
                SDL_Log("Button clicked!");
            }

            ImGui::SameLine();
            if (ImGui::Button("Close Dialog", ImVec2(120, 0)))
            {
                show_dialog = false;
            }

            ImGui::Separator();
            ImGui::Text("Background Color");

            ImGui::End();
        }
    }

    // Rendering
    ImGui::Render();

    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

    // Handle multi-viewport rendering
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}