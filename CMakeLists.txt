if(CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "android.toolchain.cmake")

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(ANDROID_ABI "arm64-v8a")
    set(ANDROID_PLATFORM "android-29")
    #message(STATUS "Using ANDROID_NDK_HOME: ${ANDROID_NDK_HOME}")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(ANDROID_ABI "arm64-v8a")
    set(ANDROID_PLATFORM "android-29")
    #message(STATUS "Using ANDROID_NDK_HOME: ${ANDROID_NDK_HOME}")
endif()

else()
endif()

cmake_minimum_required(VERSION 3.23.2)

project(VulkanFromScratch)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Debug symbols
if (MSVC)
    # MSVC (Visual Studio / clang-cl)
    add_compile_options(/utf-8)
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /MDd")
else()
    # GCC / Clang
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
endif()

# set GLM path
set(GLM_PATH "${CMAKE_SOURCE_DIR}/external/glm/0.9.9.8/include")
include_directories(${GLM_PATH})

set(FEATURE_PATH "${CMAKE_SOURCE_DIR}/feature")
include_directories(${FEATURE_PATH})

set(EDITOR_PATH "${CMAKE_SOURCE_DIR}/editors")
include_directories(${EDITOR_PATH})

# set lib path
if(CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "android.toolchain.cmake")
    set(ASSIMP_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdpartyAndroid/assimp/lib")
    set(ZLIB_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdpartyAndroid/zlib/lib")
    set(SDL_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdpartyAndroid/sdl3/lib")
    set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui")
    set(IMGUIZMO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/ImGuizmo")
    set(SPIRV_CROSS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdpartyAndroid/SPIRV-Cross/lib")
    set(EFSW_LIB_DIR "${CMAKE_SOURCE_DIR}/thirdpartyAndroid/efsw/lib")  
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(ASSIMP_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/assimp/lib")
    set(ZLIB_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/zlib/lib")
    set(SDL_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/sdl3/lib")
    set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui")
    set(IMGUIZMO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/ImGuizmo")
    set(SPIRV_CROSS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SPIRV-Cross/lib")
    set(EFSW_LIB_DIR "${CMAKE_SOURCE_DIR}/thirdparty/efsw/lib")     
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(ASSIMP_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/assimp/lib")
    set(ZLIB_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/zlib/lib")
    set(SDL_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/sdl3/lib")
    set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui")
    set(IMGUIZMO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/ImGuizmo")
    set(SPIRV_CROSS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SPIRV-Cross/lib")
    set(EFSW_LIB_DIR "${CMAKE_SOURCE_DIR}/thirdparty/efsw/lib")   
endif()

# set third party include path
set(STB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/stb")
set(ASSIMP_THIRDPARTY_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/assimp/include")
set(ASSIMP_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/assimp/include")
set(SDL_INCLUDE_DIRS "external/SDL/include")
set(SDL_SPIRV_CROSS_INCLUDE_DIRS "external/SPIRV-Cross")
set(SDL_SPIRV_CROSS_INCLUDE_DIRS1 "external/SPIRV-Cross/include")
set(EFSW_INCLUDE_DIRS "external/efsw/include")
include_directories(${CMAKE_SOURCE_DIR}/src)

# add source files from src folder
file(GLOB_RECURSE SRC_SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
    "${CMAKE_SOURCE_DIR}/editors/*.cpp"
    "${CMAKE_SOURCE_DIR}/editors/*.h"
)

# add source files from feature folder
file(GLOB_RECURSE FEATURE_SOURCES 
    "${CMAKE_SOURCE_DIR}/feature/*.cpp"
    "${CMAKE_SOURCE_DIR}/feature/*.h"
)

# add android source files
# set(GLOB_RECURSE ANDROID_SOURCES 
#     "${CMAKE_SOURCE_DIR}/androidsrc/*.cpp" 
#     "${CMAKE_SOURCE_DIR}/androidsrc/*.h"
# )

# add ImGui and ImGuizmo source files
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
    ${IMGUIZMO_DIR}/ImGuizmo.h ${IMGUIZMO_DIR}/ImGuizmo.cpp
)

# add specific source file depending on specific platform
if(ANDROID)
set(ALL_SOURCES ${SRC_SOURCES} ${FEATURE_SOURCES} ${IMGUI_SOURCES} ${ANDROID_SOURCES})
else()
# combine all source files
set(ALL_SOURCES ${SRC_SOURCES} ${FEATURE_SOURCES} ${IMGUI_SOURCES})
endif()

# add executable or library
if(ANDROID)
    add_library(${PROJECT_NAME} SHARED ${ALL_SOURCES})
else()
    add_executable(${PROJECT_NAME} ${ALL_SOURCES})
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC PROJECT_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\")
target_compile_definitions(${PROJECT_NAME} PUBLIC RESOURCE_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/resource\")

# Debug macro for logging
if(CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_GENERATOR MATCHES "Xcode" OR CMAKE_GENERATOR MATCHES "Multi-Config")
    # multiple config generators (Visual Studio, Xcode)
    message(STATUS "Multi-config generator detected: ${CMAKE_GENERATOR}")
    message(STATUS "Logging will be enabled for Debug builds, disabled for others")
    
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:RB_ENABLE_LOGGING=1>
        $<$<CONFIG:Release>:RB_ENABLE_LOGGING=0>
        $<$<CONFIG:Release>:NDEBUG=1>
        $<$<CONFIG:RelWithDebInfo>:RB_ENABLE_LOGGING=0>
        $<$<CONFIG:RelWithDebInfo>:NDEBUG=1>
        $<$<CONFIG:MinSizeRel>:RB_ENABLE_LOGGING=0>
        $<$<CONFIG:MinSizeRel>:NDEBUG=1>
    )
    
    # Android Release keep error logs
    if(ANDROID)
        target_compile_definitions(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:ANDROID_RELEASE_WITH_ERROR_LOG=1>
        )
    endif()
    
else()
    # single-config generator (Makefile, Ninja) - macOS default uses this branch
    message(STATUS "Single-config generator detected: ${CMAKE_GENERATOR}")
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(${PROJECT_NAME} PRIVATE 
            RB_ENABLE_LOGGING=1
        )
        message(STATUS "Building in DEBUG mode: logging ENABLED")
    else()
        target_compile_definitions(${PROJECT_NAME} PRIVATE 
            RB_ENABLE_LOGGING=0
            NDEBUG=1
        )
        message(STATUS "Building in RELEASE mode: logging DISABLED")
        
        # Android Release keep error logs
        if(ANDROID)
            target_compile_definitions(${PROJECT_NAME} PRIVATE ANDROID_RELEASE_WITH_ERROR_LOG=1)
        endif()
    endif()
endif()

# add Assimp header path
target_include_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_THIRDPARTY_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_INCLUDE_DIRS})

#target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR}/backends)
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUIZMO_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_SPIRV_CROSS_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_SPIRV_CROSS_INCLUDE_DIRS1})
target_include_directories(${PROJECT_NAME} PUBLIC ${EFSW_INCLUDE_DIRS})

# find Vulkan SDK
find_package(Vulkan REQUIRED)

if (Vulkan_FOUND)
    message(STATUS "Vulkan SDK found at ${Vulkan_INCLUDE_DIRS}")
    target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
else()
    message(FATAL_ERROR "Vulkan SDK not found! Please set VULKAN_SDK.")
endif()

if(CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "android.toolchain.cmake")
    #macro definition
    target_compile_definitions(${PROJECT_NAME} PRIVATE __ANDROID__=1)
    target_link_libraries(${PROJECT_NAME} ${ZLIB_LIB_DIRS}/libz.a)
    target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB_DIRS}/libassimp.a)
    target_link_libraries(${PROJECT_NAME} ${SDL_LIB_DIRS}/libSDL3.so)
    target_link_libraries(${PROJECT_NAME} ${SPIRV_CROSS_DIR}/libspirv-cross-c.a)
    target_link_libraries(${PROJECT_NAME} ${SPIRV_CROSS_DIR}/libspirv-cross-core.a)
    target_link_libraries(${PROJECT_NAME} ${SPIRV_CROSS_DIR}/libspirv-cross-glsl.a)
    target_link_libraries(${PROJECT_NAME} ${EFSW_LIB_DIR}/libefsw.a)
    target_link_libraries(${PROJECT_NAME} log)
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_definitions(${PROJECT_NAME} PRIVATE __MACOS__=1)
    target_link_libraries(${PROJECT_NAME} ${ZLIB_LIB_DIRS}/libz.a)
    target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB_DIRS}/libassimp.a)
    target_link_libraries(${PROJECT_NAME} ${SDL_LIB_DIRS}/libSDL3.dylib)
    target_link_libraries(${PROJECT_NAME} ${SPIRV_CROSS_DIR}/libspirv-cross-c.a)
    target_link_libraries(${PROJECT_NAME} ${SPIRV_CROSS_DIR}/libspirv-cross-core.a)
    target_link_libraries(${PROJECT_NAME} ${SPIRV_CROSS_DIR}/libspirv-cross-glsl.a)
    target_link_libraries(${PROJECT_NAME} ${EFSW_LIB_DIR}/libefsw.a)
    target_link_libraries(${PROJECT_NAME} "-framework CoreFoundation")
    target_link_libraries(${PROJECT_NAME} "-framework CoreServices")

elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(${PROJECT_NAME} PRIVATE __WINDOWS__=1)
    target_link_libraries(${PROJECT_NAME} ${ZLIB_LIB_DIRS}/zsd.lib)
    target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB_DIRS}/libassimp.lib)
    target_link_libraries(${PROJECT_NAME} ${SDL_LIB_DIRS}/SDL3.lib)
    target_link_libraries(${PROJECT_NAME} ${SPIRV_CROSS_DIR}/libspirv-cross-c.lib)
    target_link_libraries(${PROJECT_NAME} ${SPIRV_CROSS_DIR}/libspirv-cross-core.lib)
    target_link_libraries(${PROJECT_NAME} ${SPIRV_CROSS_DIR}/libspirv-cross-glsl.lib)
    target_link_libraries(${PROJECT_NAME} ${EFSW_LIB_DIR}/libefsw.lib)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Executable built at: $<TARGET_FILE:${PROJECT_NAME}>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL_LIB_DIRS}/SDL3.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()
target_compile_definitions(${PROJECT_NAME} PUBLIC GLM_LANG_STL11_FORCED=1)

# make sure to use C++17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# add STB
target_include_directories(${PROJECT_NAME} PUBLIC ${STB_INCLUDE_DIRS})