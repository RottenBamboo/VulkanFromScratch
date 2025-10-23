//
// Created by rottenbamboo on 2025/4/12.
//
#include "RBPipelineUtils.h"
#include <fstream>
#include <stdexcept>

namespace RottenBamboo {

#ifndef __ANDROID__
    std::vector<char> RBPipelineUtils::readFile(const std::string &filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }
        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }
#else
    std::vector<char> RBPipelineUtils::readFile(const std::string &filename) {
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", "Reading shader file: %s", filename.c_str());
    
    // sdl load file
    SDL_IOStream* file = SDL_IOFromFile(filename.c_str(), "rb");
    if (!file) {
        // try alternative path
        std::string altPath = "assets/" + filename;
        file = SDL_IOFromFile(altPath.c_str(), "rb");
        if (!file) {
            __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", 
                               "Failed to open shader file: %s, error: %s", 
                               filename.c_str(), SDL_GetError());
            throw std::runtime_error("failed to open file!");
        } else {
            __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                               "Successfully opened shader file from alt path: %s", altPath.c_str());
        }
    }
    
    // load file into memory
    Sint64 fileSize = SDL_GetIOSize(file);
    if (fileSize <= 0) {
        SDL_CloseIO(file);
        throw std::runtime_error("failed to get file size!");
    }
    
    std::vector<char> buffer(fileSize);
    size_t bytesRead = SDL_ReadIO(file, buffer.data(), fileSize);
    SDL_CloseIO(file);
    
    if (bytesRead != static_cast<size_t>(fileSize)) {
        throw std::runtime_error("failed to read entire file!");
    }
    
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                       "Shader file loaded successfully: %s, size: %zu bytes", 
                       filename.c_str(), buffer.size());
    
    return buffer;
    }
#endif
}