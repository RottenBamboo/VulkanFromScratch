cmake_minimum_required(VERSION 3.23.2)

project(VulkanFromScratch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# assimp
# set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
# set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
# set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
# set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
# set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
# set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
# set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
# set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
# set(ASSIMP_BUILD_EXTRA_POSTPROCESSING OFF CACHE BOOL "" FORCE)

# add_subdirectory(external/assimp)

# Debug symbols
if (MSVC)
    # MSVC (Visual Studio / clang-cl)
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /MDd")
else()
    # GCC / Clang
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
endif()

# set GLM path
set(GLM_PATH "${CMAKE_SOURCE_DIR}/external/glm/0.9.9.8/include")
include_directories(${GLM_PATH})
# set GLM path

set(FEATURE_PATH "${CMAKE_SOURCE_DIR}/feature")
include_directories(${FEATURE_PATH})

# set GLFW path
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(GLFW_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/glfw/MacOS/glfw-3.3.8.bin.MACOS/include")
    set(GLFW_BIN_DIRS "${CMAKE_SOURCE_DIR}/external/glfw/MacOS/glfw-3.3.8.bin.MACOS/lib-arm64")
    set(ASSIMP_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/assimp/lib")
    set(ZLIB_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/zlib/lib")
    set(SDL_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/sdl3/lib")
    set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(GLFW_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/glfw/Windows/glfw-3.3.8.bin.WIN64/include")
    set(GLFW_BIN_DIRS "${CMAKE_SOURCE_DIR}/external/glfw/Windows/glfw-3.3.8.bin.WIN64/lib-vc2022")
    set(ASSIMP_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/assimp/lib")
    set(ZLIB_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/zlib/lib")
    set(SDL_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/sdl3/lib")
    set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui")
endif()

# set STB and tinyobjloader path
set(STB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/stb")
set(TINYOBJLOADER_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/tinyobjloader")
set(ASSIMP_THIRDPARTY_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/assimp/include")
set(ASSIMP_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/assimp/include")
set(SDL_INCLUDE_DIRS "external/SDL/include")
include_directories(${CMAKE_SOURCE_DIR}/src)

# add executable file
add_executable(${PROJECT_NAME}
        src/RBPipelineUtils.cpp src/RBPipelineUtils.h
        src/RBShaderModule.cpp src/RBShaderModule.h
        src/RBApplication.cpp src/RBApplication.h
        src/RBPipelineConfig.h
        src/RBWindows.h src/RBWindows.cpp
        src/AppMain.cpp
        src/RBPipelineManager.cpp src/RBPipelineManager.h
        src/RBDevice.cpp src/RBDevice.h
        src/RBSwapChain.cpp src/RBSwapChain.h
        src/RBCommon.cpp src/RBCommon.h
        src/RBCommandBuffer.cpp src/RBCommandBuffer.h
        src/RBDescriptors.impl.h src/RBDescriptors.h
        src/RBBuffer.cpp src/RBBuffer.h
        src/RBMesh.cpp src/RBMesh.h
        src/RBDescriptorPoolManager.cpp src/RBDescriptorPoolManager.h
        src/RBDescriptorSetManager.cpp src/RBDescriptorSetManager.h
        src/RBDescriptorSetLayoutManager.cpp src/RBDescriptorSetLayoutManager.h
        src/RBPipelineLayoutManager.cpp src/RBPipelineLayoutManager.h
        src/RBImageManager.impl.h src/RBImageManager.h
        src/RBBufferManager.cpp src/RBBufferManager.h
        feature/RBGBufferPass.cpp feature/RBGBufferPass.h
        feature/RBLightingPass.cpp feature/RBLightingPass.h

        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp)

# add Assimp header path

target_include_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_THIRDPARTY_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR}/backends)
# link Assimp library
# target_link_libraries(${PROJECT_NAME} assimp)

# find Vulkan SDK
find_package(Vulkan REQUIRED)

if (Vulkan_FOUND)
    message(STATUS "Vulkan SDK found at ${Vulkan_INCLUDE_DIRS}")
    target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
else()
    message(FATAL_ERROR "Vulkan SDK not found! Please set VULKAN_SDK.")
endif()

# add GLFW
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_INCLUDE_DIRS})

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_library(GLFW_LIBRARY NAMES glfw PATHS ${GLFW_BIN_DIRS} REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARY})
    target_link_libraries(${PROJECT_NAME} ${ZLIB_LIB_DIRS}/libz.a)
    target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB_DIRS}/libassimp.a)
    target_link_libraries(${PROJECT_NAME} ${SDL_LIB_DIRS}/libSDL3.0.dylib)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} ${GLFW_BIN_DIRS}/glfw3.lib)
    target_link_libraries(${PROJECT_NAME} ${ZLIB_LIB_DIRS}/zsd.lib)
    target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB_DIRS}/assimp-vc143-mtd.lib)
    target_link_libraries(${PROJECT_NAME} ${SDL_LIB_DIRS}/SDL3.lib)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Executable built at: $<TARGET_FILE:${PROJECT_NAME}>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL_LIB_DIRS}/SDL3.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()

# add STB and tinyobjloader
target_include_directories(${PROJECT_NAME} PUBLIC ${STB_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${TINYOBJLOADER_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_INCLUDE_DIRS})