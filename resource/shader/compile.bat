
@echo off
:: Using glslc to compile Vulkan shaders
:: glslc is usually installed with the Vulkan SDK or included in the Shaderc project.

set SHADER_DIR=shaders
set OUTPUT_DIR=bin
set BUILD_ANDROID=0
set FLAGS=

if "%1"=="android" (
    set BUILD_ANDROID=1
    set FLAGS=-DANDROID --target-env=vulkan1.1
    echo Building Android version...
) else if "%1"=="" (
    echo Building Windows version...
) else (
    echo Unknown parameter: %1
    echo Usage: %~nx0 [android]
    pause
    exit /b 1
)

:: Attempting to locate glslc
where glslc >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo Error: glslc not found. Please ensure it is in the PATH or Vulkan SDK is installed.
    pause
    exit /b 1
)

mkdir "%OUTPUT_DIR%" 2>nul

:: Compile all shaders
echo Compiling shaders...
glslc %FLAGS% "gBuffer.vert" -o "bin/gBufferVert.spv"
glslc %FLAGS% "gBuffer.frag" -o "bin/gBufferFrag.spv"
glslc %FLAGS% "lighting.vert" -o "bin/lightingVert.spv"
glslc %FLAGS% "lighting.frag" -o "bin/lightingFrag.spv"
glslc %FLAGS% "sky.vert" -o "bin/skyVert.spv"
glslc %FLAGS% "sky.frag" -o "bin/skyFrag.spv"

echo Shaders compile finished!
pause