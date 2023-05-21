//
// Created by rottenbamboo on 2023/5/18.
//

#include "RBWindows.h"

namespace RottenBamboo {
    void RBWindows::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto app = reinterpret_cast<RBWindows *>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }

    RBWindows::RBWindows(uint32_t width, uint32_t height, std::string name) : width{width}, height{height}, windowName{name} {
        InitWindow();
    }

    RBWindows::~RBWindows() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void RBWindows::InitWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }
}
