// 在您的桌面工程源码目录中创建 AndroidMain.cpp
#ifdef __ANDROID__

#include "RBApplication.h"
#include <SDL3/SDL_main.h>
#include <android/log.h>
#include <exception>
#include <iostream>

extern "C" {

int SDL_main(int argc, char* argv[]) {
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", "Starting Vulkan application");
    
    RottenBamboo::RBApplication app{};
    try {
        app.run();
    }
    catch (const std::exception &e) {
        __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", "Exception: %s", e.what());
        return -1;
    }
    return 0;
}

} // extern "C"

#endif