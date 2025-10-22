#!/bin/bash

# Set directories
SHADER_DIR="shaders"
OUTPUT_DIR="bin"

# Check if glslc is available
if ! command -v glslc &> /dev/null
then
    echo "Error: glslc not found. Please ensure it's in your PATH or Vulkan SDK is installed."
    exit 1
fi

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"
TARGET=${1:-desktop}

case $TARGET in
    android)
        echo "Compiling for Android..."
        FLAGS="--target-env=vulkan1.1 -O"
        OUTPUT_SUFFIX=""
        ;;
    desktop)
        echo "Compiling for Desktop..."
        FLAGS="-O"
        OUTPUT_SUFFIX=""
        ;;
    debug)
        echo "Compiling for Debug..."
        FLAGS="-g -O0"
        OUTPUT_SUFFIX="_debug"
        ;;
    *)
        echo "Usage: $0 [android|desktop|debug]"
        exit 1
        ;;
esac

# Compile shaders
echo "Using flags: $FLAGS"

# Compile shaders
glslc $FLAGS "gBuffer.vert" -o "$OUTPUT_DIR/gBufferVert.spv"
glslc $FLAGS "gBuffer.frag" -o "$OUTPUT_DIR/gBufferFrag.spv"
glslc $FLAGS "lighting.vert" -o "$OUTPUT_DIR/lightingVert.spv"
glslc $FLAGS "lighting.frag" -o "$OUTPUT_DIR/lightingFrag.spv"
glslc $FLAGS "gBuffer.frag" -o "$OUTPUT_DIR/gBufferFrag.spv"
glslc $FLAGS "sky.vert" -o "$OUTPUT_DIR/skyVert.spv"
glslc $FLAGS "sky.frag" -o "$OUTPUT_DIR/skyFrag.spv"

echo "Shaders compiled successfully!"