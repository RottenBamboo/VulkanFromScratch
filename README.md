# VulkanFromScratch
VulkanFromScratch

## MacOS Windows Platform Request

CMake 3.23.2(or more recent)

Vulkan 1.2 (or more recent)


## Android Build Request
ndk: 29.0.14206865


## Submodule Clone

```
git submodule init
git submodule update
```

if clone submodule failed because of ssh url protocol. you can change submodule url protocol to https by these instruction below
try these
```
git config --file .gitmodules submodule.external/stb.url https://github.com/nothings/stb.git
git config --file .gitmodules submodule.external/assimp.url https://github.com/assimp/assimp.git
git config --file .gitmodules submodule.external/zlib.url https://github.com/madler/zlib.git
git config --file .gitmodules submodule.external/imgui.url https://github.com/ocornut/imgui.git
git config --file .gitmodules submodule.external/SDL.url https://github.com/libsdl-org/SDL.git
git config --file .gitmodules submodule.external/ImGuizmo.url https://github.com/CedricGuillemet/ImGuizmo.git
git config --file .gitmodules submodule.external/SPIRV-Reflect.url https://github.com/KhronosGroup/SPIRV-Reflect.git
git config --file .gitmodules submodule.external/SPIRV-Cross.url https://github.com/KhronosGroup/SPIRV-Cross.git
git submodule sync
```
then clone all submodule one more time

```
git submodule init
git submodule update
```   

## Set build script Authority

```
chmod +x external/buildZlib.sh
chmod +x external/buildAssimp.sh 
chmod +x external/buildSDL3.sh 
chmod +x external/buildSPIRV-Cross.sh 
```

## MacOS Windows Platform Build Library

```
cd external
./buildZlib.sh
./buildAssimp.sh
./buildSDL3.sh
./buildSPIRV-Cross.sh
```

## MacOS Platform Android Build Library

```
./buildZlib.sh -android -ndk ${HOME}/Library/Android/sdk/ndk/29.0.14206865
./buildAssimp.sh -android -ndk ${HOME}/Library/Android/sdk/ndk/29.0.14206865
./buildSDL3.sh -android -ndk ${HOME}/Library/Android/sdk/ndk/29.0.14206865
./buildSPIRV-Cross.sh -android -ndk ${HOME}/Library/Android/sdk/ndk/29.0.14206865
```

## Windows Platform Android Build Library

```
./buildZlib.sh -android -ndk ${HOME}\\AppData\\Local\\Android\\Sdk\\ndk\\29.0.14206865
./buildAssimp.sh -android -ndk ${HOME}\\AppData\\Local\\Android\\Sdk\\ndk\\29.0.14206865
./buildSDL3.sh -android -ndk ${HOME}\\AppData\\Local\\Android\\Sdk\\ndk\\29.0.14206865
./buildSPIRV-Cross.sh -android -ndk ${HOME}\\AppData\\Local\\Android\\Sdk\\ndk\\29.0.14206865
```

## Varies Platform Shader compile

```
./compile.sh android //android Platform
./compile.sh desktop //Windows Macos Platform
./compile.sh debug //Debug compile
```

## Windows Platform Build:
    
```
cd shader
./compile.sh
cd ..
cmake -G "Visual Studio 17 2022" -S . -B build -DCMAKE_BUILD_TYPE=Debug
cd build
cmake --build .
cd Debug
./VulkanFromScratch.exe
```

## MacOS Platform Build:

```
cd shader
./compile.sh
cd ..
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cd build
cmake --build .
./VulkanFromScratch
```
To generate XCode project remove build file and add -G Xcode into cmake instruction
```
rm -rf build 
cmake -S . -B buildMac -G Xcode -DCMAKE_BUILD_TYPE=Debug
```
If first time run xcode, please execute instruction blew to set command tool path
```
sudo xcode-select --switch /Applications/Xcode.app/Contents/Developer
xcodebuild -runFirstLaunch
```

## MacOS Platform Android Build

```
cd shader
./compile.sh android
cd ..
cmake -B buildAndroid -S . \
-DCMAKE_CXX_STANDARD=17 \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_TOOLCHAIN_FILE=${HOME}/Library/Android/sdk/ndk/29.0.14206865/build/cmake/android.toolchain.cmake \
-DANDROID_ABI=arm64-v8a \
-DANDROID_PLATFORM=android-29
cd buildAndroid
cmake --build .
```

## Windows Platform Android Build

```
cd shader
./compile.sh android
cd ..
cmake -B buildAndroid -S . \
-DCMAKE_CXX_STANDARD=17 \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_TOOLCHAIN_FILE=${HOME}\\AppData\\Local\\Android\\Sdk\\ndk\\29.0.14206865\\build\\cmake\\android.toolchain.cmake \
-DANDROID_ABI=arm64-v8a \
-DANDROID_PLATFORM=android-29 \
-G "Unix Makefiles"
cd buildAndroid
cmake --build .
```