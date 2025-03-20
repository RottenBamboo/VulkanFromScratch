cmake_minimum_required(VERSION 3.23.2)

project(VulkanFromScratch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")

# 添加标准库路径
include_directories(/Library/Developer/CommandLineTools/usr/include/c++/v1)

# 设置 GLM 路径
set(GLM_PATH "${CMAKE_SOURCE_DIR}/external/glm/0.9.9.8/include")
include_directories(${GLM_PATH})

# 设置 GLFW 路径
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(GLFW_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/glfw/MacOS/glfw-3.3.8.bin.MACOS/include")
    set(GLFW_BIN_DIRS "${CMAKE_SOURCE_DIR}/external/glfw/MacOS/glfw-3.3.8.bin.MACOS/lib-arm64")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(GLFW_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/glfw/Windows/glfw-3.3.8.bin.WIN64/include")
    set(GLFW_BIN_DIRS "${CMAKE_SOURCE_DIR}/external/glfw/Windows/glfw-3.3.8.bin.WIN64/lib-vc2022")
endif()

# 设置 STB 和 tinyobjloader 路径
set(STB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/stb")
set(TINYOBJLOADER_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/tinyobjloader")

# 添加可执行文件
add_executable(${PROJECT_NAME}
        src/RBApplication.cpp src/RBApplication.h
        src/RBWindows.h src/RBWindows.cpp
        src/AppMain.cpp
        src/RBGraphicPipeline.cpp src/RBGraphicPipeline.h
        src/RBDevice.cpp src/RBDevice.h
        src/RBSwapChain.cpp src/RBSwapChain.h
        src/RBCommon.cpp src/RBCommon.h
        src/RBCommandBuffer.cpp src/RBCommandBuffer.h
        src/RBDescriptors.cpp src/RBDescriptors.h
        src/RBBuffer.cpp src/RBBuffer.h
        src/RBMesh.cpp src/RBMesh.h
        src/RBDescriptorPoolManager.cpp src/RBDescriptorPoolManager.h
        src/RBDescriptorSetManager.cpp src/RBDescriptorSetManager.h
        src/RBDescriptorSetLayoutManager.cpp src/RBDescriptorSetLayoutManager.h
        src/RBDescriptorManager.cpp src/RBDescriptorManager.h
        src/RBPipelineLayout.cpp src/RBPipelineLayout.h
        src/RBImageManager.cpp src/RBImageManager.h
        src/RBBufferManager.cpp src/RBBufferManager.h)

# 查找 Vulkan SDK
find_package(Vulkan REQUIRED)

if (Vulkan_FOUND)
    message(STATUS "Vulkan SDK found at ${Vulkan_INCLUDE_DIRS}")
    target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
else()
    message(FATAL_ERROR "Vulkan SDK not found! Please set VULKAN_SDK.")
endif()

# 添加 GLFW
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_INCLUDE_DIRS})

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_library(GLFW_LIBRARY NAMES glfw PATHS ${GLFW_BIN_DIRS} REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARY})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} ${GLFW_BIN_DIRS}/glfw3.lib)
endif()

# 添加 STB 和 tinyobjloader
target_include_directories(${PROJECT_NAME} PUBLIC ${STB_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${TINYOBJLOADER_INCLUDE_DIRS})