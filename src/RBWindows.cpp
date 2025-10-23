//
// Created by rottenbamboo on 2023/5/18.
//

#include "RBWindows.h"
#include <stdexcept>
#include <iostream>
#include <SDL3/SDL_init.h>
#include <imgui_impl_sdl3.h>

namespace RottenBamboo {

    RBWindows::RBWindows(uint32_t width, uint32_t height, std::string name)
        : width{width}, height{height}, windowName{std::move(name)} {}

    RBWindows::~RBWindows() {
        if (window) SDL_DestroyWindow(window);
#ifndef __ANDROID__
        SDL_Quit();
#endif
    }

    void RBWindows::InitializeWindow() 
    {

    int driverCount = SDL_GetNumVideoDrivers();
    for (int i = 0; i < driverCount; ++i) 
    {
        const char* driver = SDL_GetVideoDriver(i);
        std::cout << "  - " << driver << "\n";
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    }

    std::cout << "SDL Initialized successfully!\n";

        window = SDL_CreateWindow(
            windowName.c_str(),
            width,
            height,
            SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY
        );

        if (!window) {
            throw std::runtime_error("Failed to create SDL window!");
        }

        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }

    void RBWindows::SetEventCallback(std::function<void(const SDL_Event&)> callback) {
        eventCallback = callback;
    }

    void RBWindows::PollEvents() {
        SDL_Event event;
        
        while (SDL_PollEvent(&event)) 
        {
            
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (eventCallback) 
            {
                eventCallback(event);
            }

            switch (event.type) 
            {
                case SDL_EVENT_QUIT:
                    shouldCloseFlag = true;
                    break;
                case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                    framebufferResized = true;
                    break;
                default:
                    break;
            }
        }
    }

    bool RBWindows::shouldClose() const {
        return shouldCloseFlag;
    }
}