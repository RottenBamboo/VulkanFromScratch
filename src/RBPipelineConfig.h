//
// Created by rottenbamboo on 2025/4/13.
//
#pragma once
#include <string>
#include <vulkan/vulkan_core.h>

struct RBPipelineConfig {
    std::string vertShaderPath;
    std::string fragShaderPath;
    VkPrimitiveTopology topology;
    VkPolygonMode polygonMode;
    VkCullModeFlags cullMode;
    VkFrontFace frontFace;
};