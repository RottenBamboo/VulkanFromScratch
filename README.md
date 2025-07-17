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


windows platform:
    
    cd shader

    .\compile.bat
    
    cmake -G "Visual Studio 17 2022" -S . -B build -DCMAKE_BUILD_TYPE=Debug

    cd build

    cmake --build .

    cd .\Debug\

    .\VulkanFromScratch.exe

MacOS platform:
    
    cd shader

    ./compile.sh

    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

    cd build

    cmake --build .

    ./VulkanFromScratch

    ## To generate XCode project remove build file and add -G Xcode into cmake instruction

    ## rm -rf build 

    ## cmake -S . -B build -G Xcode -DCMAKE_BUILD_TYPE=Debug

    ## If first time run xcode, please execute instruction blew to set command tool path
    ## udo xcode-select --switch /Applications/Xcode.app/Contents/Developer
    ## xcodebuild -runFirstLaunch