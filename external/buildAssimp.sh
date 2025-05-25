#!/bin/bash

set -e
OS_NAME="$(uname)"

INSTALL_DIR="$(pwd)/../thirdparty/assimp/lib"
# set the installation directory
if [[ "$OS_NAME" == "Darwin" ]]; then
    FROM_DIR="$(pwd)/assimp/lib"
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    FROM_DIR="$(pwd)/assimp/lib/Debug"
fi
# enter the script directory

cd "$(dirname "$0")/assimp"

# configure the build
if [[ "$OS_NAME" == "Darwin" ]]; then
    cmake CMakeLists.txt \
        -DBUILD_SHARED_LIBS=OFF \
        -DASSIMP_BUILD_ZLIB=OFF
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    cmake CMakeLists.txt -DASSIMP_BUILD_ASSIMP_TOOLS=ON \
        -DBUILD_SHARED_LIBS=OFF
fi

cmake --build .

cd ..
#remove dest directory
if [ -d "$INSTALL_DIR" ]; then
    :
else
    mkdir -p "$INSTALL_DIR"
fi

#rm -rf "$INSTALL_DIR"/*

if [[ "$OS_NAME" == "Darwin" ]]; then
    cp -rf "$FROM_DIR"/libassimp.a "$INSTALL_DIR"/
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    cp -rf "$FROM_DIR"/assimp-vc143-mtd.lib "$INSTALL_DIR"/
fi
#cp -rf "$FROM_DIR"/build/include "$INSTALL_DIR"/include
#cp -rf "$ZLIB_DIR" "$INSTALL_DIR"/lib