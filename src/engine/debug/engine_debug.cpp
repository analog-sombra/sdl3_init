#include "engine/debug/engine_debug.hpp"

EngineDebug::EngineDebug(SDL_Window *window, SDL_Renderer *renderer)
    : window(window), renderer(renderer)
{

    // Setup Dear ImGui Context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    // Tweak styling for viewports
    ImGuiStyle &style = ImGui::GetStyle();
    // Setup ImGui backends for SDL3 and SDL Renderer
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}

EngineDebug::~EngineDebug()
{
    // Cleanup ImGui
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void EngineDebug::NewFrame()
{
    // Start Frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void EngineDebug::Render(SDL_Renderer *renderer)
{
    ImGui::Begin("Pop-out Window");
    ImGui::Text("Drag this window outside the main application window!");
    ImGui::End();

    // Render Main Frame
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

    // Handle multi-viewport rendering
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void EngineDebug::ProcessEvent(SDL_Event *event)
{
    ImGui_ImplSDL3_ProcessEvent(event);
}