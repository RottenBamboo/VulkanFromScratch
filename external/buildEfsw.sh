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
            echo "Usage: ./buildEFSW.sh [-android] [-ndk PATH]"
            exit 1
            ;;
    esac
done

OS_NAME="$(uname)"

if [ "$BUILD_ANDROID" = true ]; then
    INSTALL_DIR="$(pwd)/../thirdpartyAndroid/efsw/lib"
else
    INSTALL_DIR="$(pwd)/../thirdparty/efsw/lib"
fi

EFSW_DIR="$(pwd)/efsw"
BUILD_DIR="$EFSW_DIR/build"

if [ -d "$BUILD_DIR" ]; then
    echo "=== Clean Build ==="
    rm -rf "$BUILD_DIR"
fi

cd "$EFSW_DIR"

if [ "$BUILD_ANDROID" = true ]; then
    echo "=== build android platform efsw static library ==="
    echo "ABI=$ANDROID_ABI, API_LEVEL=$API_LEVEL"

    cmake -S . -B build \
        -DCMAKE_TOOLCHAIN_FILE="$CMAKE_ANDROID_TOOL_CHAIN/build/cmake/android.toolchain.cmake" \
        -DANDROID_ABI="$ANDROID_ABI" \
        -DANDROID_PLATFORM="$ANDROID_PLATFORM" \
        -G "Unix Makefiles" \
        -DBUILD_SHARED_LIBS=OFF

    cmake --build build

elif [[ "$OS_NAME" == "Darwin" ]]; then
    cmake -S . -B build \
        -DBUILD_SHARED_LIBS=OFF

    cmake --build build

elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    cmake -S . -B build \
        -DCMAKE_BUILD_TYPE=Debug \
        -DBUILD_SHARED_LIBS=OFF

    cmake --build build --config Debug

else
    cmake -S . -B build \
        -DBUILD_SHARED_LIBS=OFF

    cmake --build build
fi

mkdir -p "$INSTALL_DIR"

if [ "$BUILD_ANDROID" = true ]; then
    cp -vf "$BUILD_DIR"/libefsw.a "$INSTALL_DIR"/libefsw.a
elif [[ "$OS_NAME" == "Darwin" ]]; then
    cp -vf "$BUILD_DIR"/libefsw-static.a "$INSTALL_DIR"/libefsw.a
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    cp -vf "$BUILD_DIR"/Debug/efsw-static.lib "$INSTALL_DIR"/libefsw.lib
else
    cp -vf "$BUILD_DIR"/libefsw.a "$INSTALL_DIR"/libefsw.a
fi

echo "efsw build & copy complete: $INSTALL_DIR"