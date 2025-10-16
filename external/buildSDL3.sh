#!/bin/bash

set -e

BUILD_ANDROID=false

ANDROID_ABI="arm64-v8a"
ANDROID_PLATFORM="android-29"
API_LEVEL=29
CMAKE_ANDROID_TOOL_CHAIN=""

while [[ $# -gt 0 ]]; do
    case $1 in
        -android)
            BUILD_ANDROID=true
            shift
            ;;
            -ndk)
            CMAKE_ANDROID_TOOL_CHAIN=$2
            shift 2
            ;;
        *)
            echo "未知参数: $1"
            echo "用法: ./build_assimp.sh [-android] [-ndk PATH]"
            exit 1
            ;;
    esac
done

OS_NAME="$(uname)"

INSTALL_DIR="$(pwd)/../thirdparty/sdl3/lib"
SDL_DIR="$(pwd)/SDL"
BUILD_DIR="$SDL_DIR/build"

#mkdir -p "$BUILD_DIR"
#cd "$BUILD_DIR"
cd "$SDL_DIR"

if [ "$BUILD_ANDROID" = true ]; then
    echo "=== 构建 Android 平台的 SDL3 静态库 ==="
    echo "ABI=$ANDROID_ABI, API_LEVEL=$API_LEVEL"

    cmake -S . -B build \
        -DCMAKE_TOOLCHAIN_FILE="$CMAKE_ANDROID_TOOL_CHAIN/build/cmake/android.toolchain.cmake" \
        -DANDROID_ABI="$ANDROID_ABI" \
        -DANDROID_PLATFORM="$ANDROID_PLATFORM"

    cmake --build build

elif [[ "$OS_NAME" == "Darwin" ]]; then
    cmake -S . -B build
    cmake --build build
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build --config Debug
else
    cmake -S . -B build
    cmake --build build
fi

mkdir -p "$INSTALL_DIR"

if [ "$BUILD_ANDROID" = true ]; then
    cp -vf "$BUILD_DIR"/libSDL3.so "$INSTALL_DIR"/libSDL3.0.so
elif [[ "$OS_NAME" == "Darwin" ]]; then
    cp -vf "$BUILD_DIR"/libSDL3.dylib "$INSTALL_DIR"/libSDL3.0.dylib
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    cp -vf "$BUILD_DIR"/Debug/SDL3.dll "$INSTALL_DIR"/
    cp -vf "$BUILD_DIR"/Debug/SDL3.lib "$INSTALL_DIR"/
else
    cp -vf "$BUILD_DIR"/libSDL3.dylib "$INSTALL_DIR"/libSDL3.0.dylib
fi

echo "SDL3 build & copy complete: $INSTALL_DIR"