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
#include "RBLogger.h"
int main(int argc, char* argv[]) {

    Logger::instance().setLogFile("app.log");
    RBLOG_INFO("========================================");
    RottenBamboo::RBApplication app{};
    try {
        app.run();
        RBLOG_INFO("Application exited normally");
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        RBLOG_FATAL(std::string("Fatal error: ") + e.what());
        RBLOG_FATAL("%s", e.what());
        return EXIT_FAILURE;
    }
    RBLOG_INFO("Application shutting down normally");
    RBLOG_INFO("========================================");
    return EXIT_SUCCESS;
}