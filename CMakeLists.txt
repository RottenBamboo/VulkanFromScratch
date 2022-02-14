cmake_minimum_required(VERSION 3.21)

project(VulkanFromScratch)

set(CMAKE_CXX_STANDARD 17)
set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include")
set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib")
set(Vulkan_FOUND "True")
set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")

add_executable(VulkanFromScratch src/main.cpp)

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

