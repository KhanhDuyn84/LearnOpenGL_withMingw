1. Build GLFW
    cd glfw-master -> mkdir build -> cd build
    cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
    mingw32-make

2. Build Assimp (5.0.0)
    add this line to Cmakelists.txt: SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -O3 -Wa,-mbig-obj -std=gnu++11" )
    cmake -DCMAKE_BUILD_TYPE=Release -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_TESTS=OFF -G "MinGW Makefiles" ..

3. Build SOIL
    add premake4.exe to SOIL folder
    premake4.exe gmake --platform=mingw64
    in folder SOIL/make/window: mingw32-make config=release