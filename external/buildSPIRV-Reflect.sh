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
            echo "unknown parameter: $1"
            echo "Usage: ./buildSDL3.sh [-android] [-ndk PATH]"
            exit 1
            ;;
    esac
done

OS_NAME="$(uname)"

INSTALL_DIR="$(pwd)/../thirdparty/SPIRV-Reflect/lib"
SPIRV_REFLECT_DIR="$(pwd)/SPIRV-Reflect"
BUILD_DIR="$SPIRV_REFLECT_DIR/build"

if [ -d "$BUILD_DIR" ]; then
    echo "=== Clean Build ==="
    rm -rf "$BUILD_DIR"
fi

cd "$SPIRV_REFLECT_DIR"

if [ "$BUILD_ANDROID" = true ]; then
    echo "=== build android platform SPIRV-Reflect static library ==="
    echo "ABI=$ANDROID_ABI, API_LEVEL=$API_LEVEL"

    cmake -S . -B build \
        -DCMAKE_TOOLCHAIN_FILE="$CMAKE_ANDROID_TOOL_CHAIN/build/cmake/android.toolchain.cmake" \
        -DANDROID_ABI="$ANDROID_ABI" \
        -DANDROID_PLATFORM="$ANDROID_PLATFORM" \
        -G "Unix Makefiles" \
        -DSPIRV_REFLECT_STATIC_LIB=ON

    cmake --build build

elif [[ "$OS_NAME" == "Darwin" ]]; then
    cmake -S . -B build \
    -DSPIRV_REFLECT_STATIC_LIB=ON

    cmake --build build
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug \
    -DSPIRV_REFLECT_STATIC_LIB=ON

    cmake --build build --config Debug
else
    cmake -S . -B build \
    -DSPIRV_REFLECT_STATIC_LIB=ON

    cmake --build build
fi

mkdir -p "$INSTALL_DIR"

if [ "$BUILD_ANDROID" = true ]; then
    cp -vf "$BUILD_DIR"/libspirv-reflect-static.a "$INSTALL_DIR"/libspirv-reflect-static.a
elif [[ "$OS_NAME" == "Darwin" ]]; then
    cp -vf "$BUILD_DIR"/libspirv-reflect-static.a "$INSTALL_DIR"/libspirv-reflect-static.a
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    cp -vf "$BUILD_DIR"/Debug/spirv-reflect-static.lib "$INSTALL_DIR"/libspirv-reflect-static.lib
else
    cp -vf "$BUILD_DIR"/libspirv-reflect-static.a "$INSTALL_DIR"/libspirv-reflect-static.a
fi

echo "SPIRV-Reflect build & copy complete: $INSTALL_DIR"