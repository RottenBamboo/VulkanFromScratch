#!/bin/bash

set -e

# set the installation directory
INSTALL_DIR="$(pwd)/../thirdparty/zlib/lib"
FROM_DIR="$(pwd)/zlib"
# enter the script directory

cd "$(dirname "$0")/zlib"

# configure the build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF
make

#remove dest directory
if [ -d "$INSTALL_DIR" ]; then
    :
else
    mkdir -p "$INSTALL_DIR"
fi

#rm -rf "$INSTALL_DIR"/*

cp -rf "$FROM_DIR"/build/libz.a "$INSTALL_DIR"/
#cp -rf "$FROM_DIR"/build/include "$INSTALL_DIR"/include
#cp -rf "$ZLIB_DIR" "$INSTALL_DIR"/lib