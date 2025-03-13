//
// Created by rottenbamboo on 2023/5/21.
//

#include "RBApplication.h"
#include <stdexcept>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());
    RottenBamboo::RBApplication app{};

    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    try {
        app.run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}