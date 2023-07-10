//
// Created by rottenbamboo on 2023/5/18.
//
#pragma once
#ifndef VULKANFROMSCRATCH_RBWINDOWS_H
#define VULKANFROMSCRATCH_RBWINDOWS_H
#define GLFW_INCLUDE_VULKAN

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <string>

namespace RottenBamboo {
    class RBWindows {
    public:
        GLFWwindow *window;
        bool framebufferResized = false;
    private:
        const uint32_t width;
        const uint32_t height;
        std::string windowName = "window";


    public:
        GLFWwindow* GetWindow() { return window; }

        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

        RBWindows(uint32_t width, uint32_t height, std::string name);

        ~RBWindows();

        void InitializeWindow();

        bool shouldClose() { return glfwWindowShouldClose(window); }
    };
}

#endif //VULKANFROMSCRATCH_RBWINDOWS_H
