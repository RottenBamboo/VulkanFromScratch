# VulkanFromScratch
VulkanFromScratch

Platform MacOS Windows


CMake 3.23.2(or more recent)

Vulkan 1.2 (or more recent)


git submodule init

git submodule update

find and execute chmod +x external/buildAssimp.sh cd external ./buildAssimp.sh
find and execute chmod +x external/buildZlib.sh cd external ./buildZlib.sh

windows platform:
    
    find and execute shader/compile.bat
    
    cmake -G "Visual Studio 17 2022" -S . -B build -DCMAKE_BUILD_TYPE=Debug

    cd build

    cmake --build .

    cd .\Debug\

    .\VulkanFromScratch.exe

MacOS platform:

    find and execute shader/compile.sh

    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

    cd build

    cmake --build .

    ./VulkanFromScratch