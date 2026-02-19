#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    // Defaults
    int WINDOW_WIDTH = 850;
    int WINDOW_HEIGHT = 550;

    // SDL Setup
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return 1;
    }

    // Create window with SDL_Renderer graphics context
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window = SDL_CreateWindow("DriveLink", (int)(WINDOW_WIDTH * main_scale), (int)(WINDOW_HEIGHT * main_scale), window_flags);
    if (window == nullptr) {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (renderer == nullptr) {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return 1;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
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

    c[ImGuiCol_WindowBg] = { 0.16f, 0.16f, 0.16f, 1.0f };
    c[ImGuiCol_FrameBg] = { 0.18f, 0.18f, 0.18f, 1.0f };
    c[ImGuiCol_Button] = { 0.20f, 0.20f, 0.20f, 1.0f };
    c[ImGuiCol_ButtonActive] = { 0.30f, 0.30f, 0.30f, 1.0f };
    c[ImGuiCol_ButtonHovered] = { 0.40f, 0.40f, 0.40f, 1.0f };

    // Setup scaling
    style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // App state
    bool resizable = false;
    uint8_t someCounter = 0;
    ImVec4 clear_color = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);

    // Main loop
    bool RUNNING = true;
    while (RUNNING) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            switch (event.type) {
            case SDL_EVENT_QUIT:
                RUNNING = false;
                break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                if (event.window.windowID == SDL_GetWindowID(window))
                    RUNNING = false;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                WINDOW_WIDTH = event.window.data1;
                WINDOW_HEIGHT = event.window.data2;
                printf("%dx%d\n", event.window.data1, event.window.data2);
                break;
            }

#ifdef _DEBUG
            // Quit when pressing Q while debugging to prevent reaching for the close button all the time
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_Q)
                RUNNING = false;

            // Do something on pressing a key, can do any arbitrary thing on arbitrary key
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_E) {
                resizable = !resizable;
                printf("%s\n", resizable ? "yes" : "no");
            }
#endif
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // The main window
        ImGui::Begin(
            "DriveLink",
            NULL,
            (resizable ? 0 : ImGuiWindowFlags_NoResize)
                | ImGuiWindowFlags_NoTitleBar
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_UnsavedDocument
        );

        ImGui::BeginChild("LeftPanel", { 200, 200 }, 0);
        if (ImGui::Button("Click Me!")) {
            someCounter++;
        }
        ImGui::Text("You have hello the world %d times!", someCounter);
        ImGui::EndChild();

        ImGui::End();

        // Rendering
        ImGui::Render();
        SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}