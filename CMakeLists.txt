cmake_minimum_required(VERSION 3.23.2)

project(VulkanFromScratch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")
set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include")
set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib")
set(Vulkan_FOUND "True")
set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
set(STB_INCLUDE_DIRS "external/stb")
set(TINYOBJLOADER_INCLUDE_DIRS "external/tinyobjloader")

add_executable(VulkanFromScratch src/RBApplication.cpp src/RBApplication.h src/RBWindows.h src/RBWindows.cpp src/AppMain.cpp src/RBGraphicPipeline.cpp src/RBGraphicPipeline.h src/RBDevice.cpp src/RBDevice.h src/RBSwapChain.cpp src/RBSwapChain.h src/RBCommon.cpp src/RBCommon.h src/RBCommandBuffer.cpp src/RBCommandBuffer.h src/RBDescriptors.cpp src/RBDescriptors.h src/RBBuffer.cpp src/RBBuffer.h src/RBMesh.cpp src/RBMesh.h)

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

find_package(glm CONFIG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLM_PATH})
target_link_libraries(${PROJECT_NAME} glm::glm)


find_package(glfw3 CONFIG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${STB_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${TINYOBJLOADER_INCLUDE_DIRS})