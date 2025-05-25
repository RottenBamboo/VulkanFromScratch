#!/bin/bash

set -e

# set the installation directory
INSTALL_DIR="$(pwd)/../thirdparty/assimp"
FROM_DIR="$(pwd)/assimp"
# enter the script directory

cd "$(dirname "$0")/assimp"

# configure the build
cmake CMakeLists.txt \
    -DBUILD_SHARED_LIBS=OFF \
    -DASSIMP_BUILD_ZLIB=OFF

cmake --build .


#remove dest directory
if [ -d "$INSTALL_DIR" ]; then
    :
else
    mkdir -p "$INSTALL_DIR"
fi

#rm -rf "$INSTALL_DIR"/*

cp -rf "$FROM_DIR"/lib "$INSTALL_DIR"/lib
#cp -rf "$FROM_DIR"/build/include "$INSTALL_DIR"/include
#cp -rf "$ZLIB_DIR" "$INSTALL_DIR"/lib