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
#include <unistd.h>  // POSIX API
#endif

int main(int argc, char* argv[]) {
    if (argc < 1) {
        std::cerr << "No executable path provided!" << std::endl;
        return -1;
    }

    // 获取可执行文件的路径
    std::string executablePath = argv[0];
    size_t lastSlash = executablePath.find_last_of("\\/");
    if (lastSlash != std::string::npos) {
        executablePath = executablePath.substr(0, lastSlash);
    }

    // 设置工作目录
#ifdef _WIN32
    if (!SetCurrentDirectory(executablePath.c_str())) {
        std::cerr << "Failed to change working directory!" << std::endl;
        return -1;
    }
#else
    if (chdir(executablePath.c_str()) != 0) {
        std::cerr << "Failed to change working directory!" << std::endl;
        return -1;
    }
#endif

    // 输出当前工作目录
#ifdef _WIN32
    char currentDir[MAX_PATH];
    if (GetCurrentDirectory(MAX_PATH, currentDir)) {
        std::cout << "Current working directory: " << currentDir << std::endl;
    }
#else
    char currentDir[1024];
    if (getcwd(currentDir, sizeof(currentDir)) != nullptr) {
        std::cout << "Current working directory: " << currentDir << std::endl;
    }
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