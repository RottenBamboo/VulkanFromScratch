#!/bin/bash

set -e
OS_NAME="$(uname)"

# set the installation directory
INSTALL_DIR="$(dirname "$0")/../thirdparty/zlib/lib"
FROM_DIR="$(dirname "$0")/zlib"
# enter the script directory

cd "$(dirname "$0")/zlib"

# configure the build

if [ -d "build" ]; then
    :
else
    mkdir -p build
fi

cd build
cmake .. -DBUILD_SHARED_LIBS=OFF
cmake --build .
cd ../..

#remove dest directory
if [ -d "$INSTALL_DIR" ]; then
    :
else
    mkdir -p "$INSTALL_DIR"
fi

#rm -rf "$INSTALL_DIR"/*


if [[ "$OS_NAME" == "Darwin" ]]; then
    echo "macOS"
    cp -rf "$FROM_DIR"/build/libz.a "$INSTALL_DIR"/
elif [[ "$OS_NAME" == "MINGW"* || "$OS_NAME" == "MSYS"* || "$OS_NAME" == "CYGWIN"* ]]; then
    echo "Windows（Git Bash / WSL）"
    cp -rf "$FROM_DIR"/build/Debug/zsd.lib "$INSTALL_DIR"/
fi
#cp -rf "$FROM_DIR"/build/include "$INSTALL_DIR"/include
#cp -rf "$ZLIB_DIR" "$INSTALL_DIR"/lib