cmake_minimum_required(VERSION 3.23.2)

project(VulkanFromScratch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")
set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include")
set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib")
set(Vulkan_FOUND "True")
set(GLM_PATH "external/glm/0.9.9.8/include")
set(GLM_LIBRARIES_PATH "external/glm/0.9.9.8/lib/cmake/glm")

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(GLFW_INCLUDE_DIRS "external/glfw/MacOS/glfw-3.3.8.bin.MACOS/include")
    set(GLFW_BIN_DIRS "external/glfw/MacOS/glfw-3.3.8.bin.MACOS/lib-arm64")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(GLFW_INCLUDE_DIRS "external/glfw/Windows/glfw-3.3.8.bin.WIN64/include")
    set(GLFW_BIN_DIRS "external/glfw/Windows/glfw-3.3.8.bin.WIN64/lib-vc2022")
endif()

set(STB_INCLUDE_DIRS "external/stb")
set(TINYOBJLOADER_INCLUDE_DIRS "external/tinyobjloader")

include_directories(${PROJECT_NAME} PUBLIC ${GLM_PATH})

add_executable(${PROJECT_NAME} src/RBApplication.cpp src/RBApplication.h
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
        src/RBImageManager.cpp src/RBImageManager.h)

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_INCLUDE_DIRS})

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/${GLFW_BIN_DIRS}/libglfw.3.dylib)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/${GLFW_BIN_DIRS}/glfw3.lib)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${STB_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${TINYOBJLOADER_INCLUDE_DIRS})