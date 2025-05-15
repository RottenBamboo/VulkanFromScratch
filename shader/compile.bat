bat
@echo off
:: Using glslc to compile Vulkan shaders
:: glslc is usually installed with the Vulkan SDK or included in the Shaderc project.

set SHADER_DIR=shaders
set OUTPUT_DIR=shaders/spv

:: Attempting to locate glslc
where glslc >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo Error: glslc not found. Please ensure it is in the PATH or Vulkan SDK is installed.
    pause
    exit /b 1
)

:: Create output directory
::if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

:: Compile all shaders
glslc "gBuffer.vert" -o "gBufferVert.spv"
glslc "gBuffer.frag" -o "gBufferFrag.spv"
glslc "lighting.vert" -o "lightingVert.spv"
glslc "lighting.frag" -o "lightingFrag.spv"

echo Shaders 编译完成!
pause