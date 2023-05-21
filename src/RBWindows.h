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
    private:
        GLFWwindow *window;
        const uint32_t width;
        const uint32_t height;
        std::string windowName = "window";
        bool framebufferResized = false;

        void InitWindow();

    public:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

        RBWindows(uint32_t width, uint32_t height, std::string name);

        ~RBWindows();
        bool shouldClose() { return glfwWindowShouldClose(window); }
    };
}

#endif //VULKANFROMSCRATCH_RBWINDOWS_H
