#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "InputState.hpp"
#include "Server.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;

    uint16_t windowHeight;
    uint16_t windowWidth;

    int someCounter;
} AppState;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {

    // App state creation
    *appstate = new AppState {};
    AppState& state = *static_cast<AppState*>(*appstate);

    state.windowWidth = 550;
    state.windowHeight = 350;

    // SDL Setup
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Create window with SDL_Renderer graphics context
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window = SDL_CreateWindow("DriveLink", (int)(state.windowWidth * main_scale), (int)(state.windowHeight * main_scale), window_flags);
    if (window == nullptr) {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    state.window = window;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (renderer == nullptr) {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    state.renderer = renderer;
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    static ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.IniFilename = NULL;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    // Styling
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.WindowBorderSize = 1;
    style.FrameBorderSize = 1;

    ImGui::StyleColorsDark();
    auto& c = ImGui::GetStyle().Colors;

    c[ImGuiCol_WindowBg] = { 0.0f, 0.0f, 0.0f, 0.0f }; // Transparent windows to prevent overlay over things drawn with SDL
    c[ImGuiCol_FrameBg] = { 0.18f, 0.18f, 0.18f, 1.0f };
    c[ImGuiCol_Button] = { 0.20f, 0.20f, 0.20f, 1.0f };
    c[ImGuiCol_ButtonActive] = { 0.30f, 0.30f, 0.30f, 1.0f };
    c[ImGuiCol_ButtonHovered] = { 0.40f, 0.40f, 0.40f, 1.0f };

    // Setup scaling
    style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // Initialise sockpp
    sockpp::initialize();

    // Initialise and start the UDP input event listener
    static dl::network::InputListener inputServer = dl::network::InputListener(
        sockpp::inet_address(
            "0.0.0.0", 7001
        )
    );
    inputServer.Start();

    // Initialise the Input State singleton
    dl::InputState::GetInstance();

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    AppState& state = *static_cast<AppState*>(appstate);

    ImGui_ImplSDL3_ProcessEvent(event);
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
        break;

    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        if (event->window.windowID == SDL_GetWindowID(state.window))
            return SDL_APP_SUCCESS;
        break;

    case SDL_EVENT_WINDOW_RESIZED:
        state.windowWidth = event->window.data1;
        state.windowHeight = event->window.data2;
        break;

    case SDL_EVENT_KEY_DOWN:

#ifdef _DEBUG
        // Quit when pressing Q while debugging to prevent reaching for the close button all the time
        if (event->key.scancode == SDL_SCANCODE_Q)
            return SDL_APP_SUCCESS;

        // Do something on pressing a key, can do any arbitrary thing on arbitrary key
        if (event->key.scancode == SDL_SCANCODE_E) {
        }
#endif
        break;
    }

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame*/
SDL_AppResult SDL_AppIterate(void* appstate) {

    ImGuiIO& io = ImGui::GetIO();
    uint64_t now = SDL_GetTicks();
    AppState& state = *static_cast<AppState*>(appstate);

    // Clear screen
    SDL_SetRenderDrawColorFloat(state.renderer, 0.2f, 0.2f, 0.2f, 1.f);
    SDL_RenderClear(state.renderer);

    // Normal SDL Rendering
    /*
        SDL Rendering here...
    */

    // Start ImGui rendering
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // ImGui window
    ImGui::SetNextWindowSize({ io.DisplaySize.x, io.DisplaySize.y }, ImGuiCond_Once);
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::Begin(
        "DriveLink",
        NULL,
        ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
    );

    ImGui::BeginChild(
        "LeftPanel", { 300, 200 },
        ImGuiChildFlags_Border
    );

    ImGui::Text("Steering: %f", dl::InputState::GetInstance().getSteering());

    ImGui::EndChild();

    ImGui::End();

    // Rendering
    ImGui::Render();
    SDL_SetRenderScale(state.renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), state.renderer);

    SDL_RenderPresent(state.renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    AppState& state = *static_cast<AppState*>(appstate);

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}