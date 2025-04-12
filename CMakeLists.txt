cmake_minimum_required(VERSION 3.23.2)

project(VulkanFromScratch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

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
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(GLFW_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/glfw/Windows/glfw-3.3.8.bin.WIN64/include")
    set(GLFW_BIN_DIRS "${CMAKE_SOURCE_DIR}/external/glfw/Windows/glfw-3.3.8.bin.WIN64/lib-vc2022")
endif()

# set STB and tinyobjloader path
set(STB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/stb")
set(TINYOBJLOADER_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/tinyobjloader")
include_directories(${CMAKE_SOURCE_DIR}/src)

# add executable file
add_executable(${PROJECT_NAME}
        src/RBPipelineUtils.cpp src/RBPipelineUtils.h
        src/RBShaderModule.cpp src/RBShaderModule.h
        src/RBApplication.cpp src/RBApplication.h
        src/RBPipelineConfig.h
        src/RBWindows.h src/RBWindows.cpp
        src/AppMain.cpp
        src/RBGraphicPipelineManager.cpp src/RBGraphicPipelineManager.h
        src/RBPipelineManager.cpp src/RBPipelineManager.h
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
        src/RBPipelineLayoutManager.cpp src/RBPipelineLayoutManager.h
        src/RBImageManager.cpp src/RBImageManager.h
        src/RBBufferManager.cpp src/RBBufferManager.h
        feature/RBLightingPass.cpp feature/RBLightingPass.h)

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
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} ${GLFW_BIN_DIRS}/glfw3.lib)
endif()

# add STB and tinyobjloader
target_include_directories(${PROJECT_NAME} PUBLIC ${STB_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${TINYOBJLOADER_INCLUDE_DIRS})