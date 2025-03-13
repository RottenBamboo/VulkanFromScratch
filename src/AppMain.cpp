//
// Created by rottenbamboo on 2023/5/21.
//

#include "RBApplication.h"
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h> // Windows API
#else
#include <filesystem>
#endif

int main(int argc, char* argv[]) {

#ifdef _WIN32
    if (argc < 1) {
        std::cerr << "No executable path provided!" << std::endl;
        return -1;
    }

    std::string executablePath = argv[0];
    size_t lastSlash = executablePath.find_last_of("\\/");
    if (lastSlash != std::string::npos) {
        executablePath = executablePath.substr(0, lastSlash);
    }

    for (int i = 0; i < 1; ++i) { // 两次 parent_path
        size_t parentSlash = executablePath.find_last_of("\\/");
        if (parentSlash != std::string::npos) {
            executablePath = executablePath.substr(0, parentSlash);
        } else {
            std::cerr << "Failed to get parent directory!" << std::endl;
            return -1;
        }
    }
    if (!SetCurrentDirectory(executablePath.c_str())) {
        std::cerr << "Failed to change working directory!" << std::endl;
        return -1;
    }
    char currentDir[MAX_PATH];
    if (GetCurrentDirectory(MAX_PATH, currentDir)) {
        std::cout << "Current working directory: " << currentDir << std::endl;
    }
#else
    std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
#endif

    RottenBamboo::RBApplication app{};

    try {
        app.run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}