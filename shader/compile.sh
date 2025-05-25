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

# Compile shaders
glslc "gBuffer.vert" -o "$OUTPUT_DIR/gBufferVert.spv"
glslc "gBuffer.frag" -o "$OUTPUT_DIR/gBufferFrag.spv"
glslc "lighting.vert" -o "$OUTPUT_DIR/lightingVert.spv"
glslc "lighting.frag" -o "$OUTPUT_DIR/lightingFrag.spv"

echo "Shaders compiled successfully!"