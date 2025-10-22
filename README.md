# VulkanFromScratch
VulkanFromScratch

Platform MacOS Windows


CMake 3.23.2(or more recent)

Vulkan 1.2 (or more recent)


git submodule init

git submodule update


chmod +x external/buildZlib.sh

chmod +x external/buildAssimp.sh 

chmod +x external/buildSDL3.sh 


cd external 

./buildZlib.sh

./buildAssimp.sh

./buildSDL3.sh


MacOS platform Android Build

./buildZlib.sh -android -ndk ${HOME}/Library/Android/sdk/ndk/29.0.14206865

./buildAssimp.sh -android -ndk ${HOME}/Library/Android/sdk/ndk/29.0.14206865

./buildSDL3.sh -android -ndk ${HOME}/Library/Android/sdk/ndk/29.0.14206865


windows platform:
    
    cd shader

    .\compile.bat

    cd ..
    
    cmake -G "Visual Studio 17 2022" -S . -B build -DCMAKE_BUILD_TYPE=Debug

    cd build

    cmake --build .

    cd .\Debug\

    .\VulkanFromScratch.exe

MacOS platform:
    
    cd shader

    ./compile.sh

    cd ..

    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

    cd build

    cmake --build .

    ./VulkanFromScratch

    ## To generate XCode project remove build file and add -G Xcode into cmake instruction

    ## rm -rf build 

    ## cmake -S . -B buildMac -G Xcode -DCMAKE_BUILD_TYPE=Debug

    ## If first time run xcode, please execute instruction blew to set command tool path
    ## sudo xcode-select --switch /Applications/Xcode.app/Contents/Developer
    ## xcodebuild -runFirstLaunch

MacOS platform Android Build
    
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