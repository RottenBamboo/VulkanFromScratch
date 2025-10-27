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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
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
# set GLM path

set(FEATURE_PATH "${CMAKE_SOURCE_DIR}/feature")
include_directories(${FEATURE_PATH})

# set lib path
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(ASSIMP_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/assimp/lib")
    set(ZLIB_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/zlib/lib")
    set(SDL_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/sdl3/lib")
    set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui")
    set(IMGUIZMO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/ImGuizmo")
    set(SPIRV_REFLECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SPIRV-Reflect/lib")
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(ASSIMP_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/assimp/lib")
    set(ZLIB_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/zlib/lib")
    set(SDL_LIB_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/sdl3/lib")
    set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui")
    set(IMGUIZMO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/ImGuizmo")
    set(SPIRV_REFLECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SPIRV-Reflect/lib")
endif()

# set third party include path
set(STB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/stb")
set(ASSIMP_THIRDPARTY_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/thirdparty/assimp/include")
set(ASSIMP_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/assimp/include")
set(SDL_INCLUDE_DIRS "external/SDL/include")
set(SDL_SPIRV_REFLECT_INCLUDE_DIRS "external/SPIRV-Reflect")
set(SDL_SPIRV_REFLECT_INCLUDE_DIRS1 "external/SPIRV-Reflect/include")
include_directories(${CMAKE_SOURCE_DIR}/src)

# add executable file
if(ANDROID)
add_library(${PROJECT_NAME} SHARED
        src/RBPipelineUtils.cpp src/RBPipelineUtils.h
        src/RBShaderModule.cpp src/RBShaderModule.h
        src/RBApplication.cpp src/RBApplication.h
        src/RBPipelineConfig.h
        src/RBWindows.h src/RBWindows.cpp
        src/AppMain.cpp
        src/AppMainAndroid.cpp
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
        src/RBGUI.cpp src/RBGUI.h
        src/RBBufferManager.cpp src/RBBufferManager.h
        src/RBCamera.h src/RBCameraManager.h
        src/RBRuntimeCameraManager.cpp src/RBRuntimeCameraManager.h
        src/RBEditorCameraManager.cpp src/RBEditorCameraManager.h
        src/RBRenderer.cpp src/RBRenderer.h
        src/RBMesh.cpp src/RBMesh.h
        src/RBMaterial.cpp src/RBMaterial.h
        src/RBMaterialUtils.h
        src/RBModel.cpp src/RBModel.h
        src/RBResource.h
        src/RBShader.cpp src/RBShader.h
        src/RBResourceManager.cpp src/RBResourceManager.h
        feature/RBGBufferPass.cpp feature/RBGBufferPass.h
        feature/RBLightingPass.cpp feature/RBLightingPass.h
        feature/RBSkyPass.cpp feature/RBSkyPass.h

        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
        ${IMGUIZMO_DIR}/ImGuizmo.h ${IMGUIZMO_DIR}/ImGuizmo.cpp
        )
else()
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
        src/RBGUI.cpp src/RBGUI.h
        src/RBBufferManager.cpp src/RBBufferManager.h
        src/RBCamera.h src/RBCameraManager.h
        src/RBRuntimeCameraManager.cpp src/RBRuntimeCameraManager.h
        src/RBEditorCameraManager.cpp src/RBEditorCameraManager.h
        src/RBRenderer.cpp src/RBRenderer.h
        src/RBMesh.cpp src/RBMesh.h
        src/RBMaterial.cpp src/RBMaterial.h
        src/RBMaterialUtils.h
        src/RBModel.cpp src/RBModel.h
        src/RBResource.h
        src/RBShader.cpp src/RBShader.h
        src/RBResourceManager.cpp src/RBResourceManager.h
        feature/RBGBufferPass.cpp feature/RBGBufferPass.h
        feature/RBLightingPass.cpp feature/RBLightingPass.h
        feature/RBSkyPass.cpp feature/RBSkyPass.h

        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
        ${IMGUIZMO_DIR}/ImGuizmo.h ${IMGUIZMO_DIR}/ImGuizmo.cpp
        )
endif()


# add Assimp header path
target_compile_definitions(${PROJECT_NAME} PUBLIC PROJECT_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_THIRDPARTY_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_INCLUDE_DIRS})

#target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR}/backends)
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUIZMO_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_SPIRV_REFLECT_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_SPIRV_REFLECT_INCLUDE_DIRS1})

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
    target_compile_definitions(${PROJECT_NAME} PUBLIC SPIRV_REFLECT_USE_SYSTEM_SPIRV_H)
    target_link_libraries(${PROJECT_NAME} ${ZLIB_LIB_DIRS}/libz.a)
    target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB_DIRS}/libassimp.a)
    target_link_libraries(${PROJECT_NAME} ${SDL_LIB_DIRS}/libSDL3.0.so)
    target_link_libraries(${PROJECT_NAME} ${SPIRV_REFLECT_DIR}/libspirv-reflect-static.a)
    target_link_libraries(${PROJECT_NAME} log)
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_definitions(${PROJECT_NAME} PUBLIC SPIRV_REFLECT_USE_SYSTEM_SPIRV_H)
    target_link_libraries(${PROJECT_NAME} ${ZLIB_LIB_DIRS}/libz.a)
    target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB_DIRS}/libassimp.a)
    target_link_libraries(${PROJECT_NAME} ${SPIRV_REFLECT_DIR}/libspirv-reflect-static.a)
    target_link_libraries(${PROJECT_NAME} ${SDL_LIB_DIRS}/libSDL3.0.dylib)

elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(${PROJECT_NAME} PUBLIC SPIRV_REFLECT_USE_SYSTEM_SPIRV_H)
    target_link_libraries(${PROJECT_NAME} ${ZLIB_LIB_DIRS}/zsd.lib)
    target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB_DIRS}/assimp-vc143-mtd.lib)
    target_link_libraries(${PROJECT_NAME} ${SDL_LIB_DIRS}/SDL3.lib)
    target_link_libraries(${PROJECT_NAME} ${SPIRV_REFLECT_DIR}/libspirv-reflect-static.lib)
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