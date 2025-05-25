#!/bin/bash

set -e

# set the installation directory
INSTALL_DIR="$(pwd)/../thirdparty/assimp"
FROM_DIR="$(pwd)/assimp/build"
ZLIB_DIR="$(pwd)/assimp/build/contrib/zlib/Debug"
# enter the script directory
cd "$(dirname "$0")/assimp"

# create build directory
mkdir -p build

mkdir -p "$INSTALL_DIR"

# configure the build
cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
    -DASSIMP_BUILD_TESTS=OFF \
    -DASSIMP_BUILD_SHARED_LIBS=OFF \
    -DASSIMP_BUILD_ASSIMP_TOOLS=OFF \
    -DASSIMP_BUILD_SAMPLES=OFF \
    -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=OFF \
    -DASSIMP_BUILD_OBJ_IMPORTER=ON \
    -DBUILD_SHARED_LIBS=OFF \
    -DASSIMP_BUILD_ZLIB=ON \ 

cd build

# build the project
cmake --build .

rm -rf "$INSTALL_DIR"/*

cp -rf "$FROM_DIR"/lib "$INSTALL_DIR"/lib
cp -rf "$FROM_DIR"/include "$INSTALL_DIR"/include
cp -rf "$ZLIB_DIR" "$INSTALL_DIR"/lib