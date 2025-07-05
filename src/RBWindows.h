//
// Created by rottenbamboo on 2023/5/18.
//
#pragma once

#include <SDL.h>
#include <SDL_vulkan.h>
#include <string>
#include <functional>

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
        void SetEventCallback(std::function<void(const SDL_Event&)> callback);
        void PollEvents();
        bool shouldClose() const;
        
    private:
        std::function<void(const SDL_Event&)> eventCallback = nullptr;
    };
}