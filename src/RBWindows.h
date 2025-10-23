//
// Created by rottenbamboo on 2023/5/18.
//
#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <string>
#include <functional>

#ifdef __ANDROID__
#include <android/log.h>
// Android log macro
#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", __VA_ARGS__)
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", __VA_ARGS__)
#else
// desktop log macro
#define LOG_INFO(...) std::cout << __VA_ARGS__ << std::endl
#define LOG_ERROR(...) std::cerr << __VA_ARGS__ << std::endl
#endif

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