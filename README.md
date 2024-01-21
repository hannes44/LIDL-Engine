# GameEngineTDA572

Setup:
1. Disable compression since GLM is large and can cause issues during download: `git config --global core.compression 0`
2. Clone the repository with `git clone --recurse-submodules https://github.com/hannes44/GameEngineTDA572.git`
3. Install [CMake](https://cmake.org/download/)
4. Run cmake at the top folder layer. For example: `cmake -G "Visual Studio 17 2022" -A x64 -S . -B build`
5. Go to the generated build folder and open `Engine.sln` in Visual Studio 2022
6. Right click `Engine` in the Solution explorer and select `Set as Startup Project`
