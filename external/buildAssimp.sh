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

INSTALL_DIR="$(pwd)/../thirdparty/assimp/lib"
# set the installation directory
if [ "$BUILD_ANDROID" = true ]; then
    FROM_DIR="$(pwd)/assimp/lib"
elif [[ "$OS_NAME" == "Darwin" ]]; then
    FROM_DIR="$(pwd)/assimp/lib"
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    FROM_DIR="$(pwd)/assimp/lib/Debug"
fi
# enter the script directory
cd "$(dirname "$0")/assimp"

if [ -f "CMakeCache.txt" ]; then
    rm -f CMakeCache.txt
fi
if [ -d "CMakeFiles" ]; then
    rm -rf CMakeFiles
fi

# configure the build
if [ "$BUILD_ANDROID" = true ]; then
    echo "=== Build android platform assimp static library ==="
    echo "ABI=$ANDROID_ABI, API_LEVEL=$API_LEVEL"

    cmake CMakeLists.txt \
        -DCMAKE_TOOLCHAIN_FILE="$CMAKE_ANDROID_TOOL_CHAIN/build/cmake/android.toolchain.cmake" \
        -DANDROID_ABI=arm64-v8a \
        -DANDROID_PLATFORM=android-29 \
        -G "Unix Makefiles" \
        -DBUILD_SHARED_LIBS=OFF \
        -DASSIMP_BUILD_ZLIB=ON \
        -DZLIB_LIBRARY="$(pwd)/../thirdparty/zlib/lib/libz.a" \
        -DASSIMP_BUILD_ASSIMP_TOOLS=OFF \

elif [[ "$OS_NAME" == "Darwin" ]]; then
    cmake CMakeLists.txt \
        -DBUILD_SHARED_LIBS=OFF \
        -DASSIMP_BUILD_ZLIB=OFF
        
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    cmake CMakeLists.txt -DASSIMP_BUILD_ASSIMP_TOOLS=ON \
        -DBUILD_SHARED_LIBS=OFF
fi

cmake --build .
echo "=== 开始构建 ==="

cd ..
#remove dest directory
if [ -d "$INSTALL_DIR" ]; then
    :
else
    mkdir -p "$INSTALL_DIR"
fi

#rm -rf "$INSTALL_DIR"/*

if [ "$BUILD_ANDROID" = true ]; then
    cp -rf "$FROM_DIR"/libassimp.a "$INSTALL_DIR"/
elif [[ "$OS_NAME" == "Darwin" ]]; then
    cp -rf "$FROM_DIR"/libassimp.a "$INSTALL_DIR"/
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    cp -rf "$FROM_DIR"/assimp-vc143-mtd.lib "$INSTALL_DIR"/
fi
#cp -rf "$FROM_DIR"/build/include "$INSTALL_DIR"/include
#cp -rf "$ZLIB_DIR" "$INSTALL_DIR"/lib