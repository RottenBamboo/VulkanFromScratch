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
            echo "Usage: ./buildZlib.sh [-android] [-ndk PATH]"
            exit 1
            ;;
    esac
done

OS_NAME="$(uname)"

# set the installation directory
INSTALL_DIR="$(dirname "$0")/../thirdparty/zlib/lib"
FROM_DIR="$(dirname "$0")/zlib"
BUILD_DIR="$FROM_DIR/build"
# enter the script directory

cd "$FROM_DIR"

# configure the build


if [ -f "CMakeCache.txt" ]; then
    rm -f CMakeCache.txt
fi
if [ -d "CMakeFiles" ]; then
    rm -rf CMakeFiles
fi

if [ -d "$BUILD_DIR" ]; then
    echo "=== Clean Build ==="
    rm -rf "$BUILD_DIR"
fi

if [ -d "build" ]; then
    :
else
    mkdir -p build
fi


cd build


if [ "$BUILD_ANDROID" = true ]; then
    echo "=== Build android platform zlib static library ==="
    echo "ABI=$ANDROID_ABI, API_LEVEL=$API_LEVEL"

    cmake .. \
        -DCMAKE_TOOLCHAIN_FILE="$CMAKE_ANDROID_TOOL_CHAIN/build/cmake/android.toolchain.cmake" \
        -DANDROID_ABI="$ANDROID_ABI" \
        -DANDROID_PLATFORM="$ANDROID_PLATFORM" \
        -G "Unix Makefiles" \
        -DBUILD_SHARED_LIBS=OFF
else        
cmake .. -DBUILD_SHARED_LIBS=OFF
fi
cmake --build .
cd ../..

#remove dest directory
if [ -d "$INSTALL_DIR" ]; then
    :
else
    mkdir -p "$INSTALL_DIR"
fi

#rm -rf "$INSTALL_DIR"/*

if [ "$BUILD_ANDROID" = true ]; then
    cp -rf "$FROM_DIR"/build/libz.a "$INSTALL_DIR"/
elif [[ "$OS_NAME" == "Darwin" ]]; then
    echo "macOS"
    cp -rf "$FROM_DIR"/build/libz.a "$INSTALL_DIR"/
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    echo "Windows(Git Bash / WSL)"
    cp -rf "$FROM_DIR"/build/Debug/zsd.lib "$INSTALL_DIR"/
fi
#cp -rf "$FROM_DIR"/build/include "$INSTALL_DIR"/include
#cp -rf "$ZLIB_DIR" "$INSTALL_DIR"/lib