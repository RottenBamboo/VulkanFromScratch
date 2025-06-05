//
// Created by rottenbamboo on 2023/5/18.
//
#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <string>

namespace RottenBamboo {

    class RBWindows {
    public:
        SDL_Window *window = nullptr;
        bool framebufferResized = false;

    private:
        uint32_t width;
        uint32_t height;
        std::string windowName;
        bool shouldCloseFlag = false;

    public:
        RBWindows(uint32_t width, uint32_t height, std::string name);
        ~RBWindows();

        SDL_Window* GetWindow() { return window; }

        void InitializeWindow();
        void PollEvents();
        bool shouldClose() const;
    };
}