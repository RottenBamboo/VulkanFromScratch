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