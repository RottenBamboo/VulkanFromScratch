#!/bin/bash

set -e
OS_NAME="$(uname)"

INSTALL_DIR="$(pwd)/../thirdparty/sdl3/lib"
SDL_DIR="$(pwd)/SDL"
BUILD_DIR="$SDL_DIR/build"

#mkdir -p "$BUILD_DIR"
#cd "$BUILD_DIR"
cd "$SDL_DIR"

if [[ "$OS_NAME" == "Darwin" ]]; then
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

if [[ "$OS_NAME" == "Darwin" ]]; then
    cp -vf "$BUILD_DIR"/libSDL3.dylib "$INSTALL_DIR"/libSDL3.0.dylib
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    cp -vf "$BUILD_DIR"/Debug/SDL3.dll "$INSTALL_DIR"/
else
    cp -vf "$BUILD_DIR"/libSDL3.dylib "$INSTALL_DIR"/libSDL3.0.dylib
fi

echo "SDL3 build & copy complete: $INSTALL_DIR"