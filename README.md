# LIDL Engine

Setup:
1. Clone the repository with `git clone --recurse-submodules https://github.com/hannes44/LIDL-Engine.git`
2. Install [CMake](https://cmake.org/download/)
3. Run cmake at the top folder layer. For example: `cmake -G "Visual Studio 17 2022" -A x64 -S . -B build`
4. Go to the generated build folder and open `GameEngine.sln` in Visual Studio 2022
5. Right click either `Editor`, `Game` or `Game2` depending on what you want to run in the Solution explorer and select `Set as Startup Project`
6. Go to `Tools -> Options -> Text Editor -> Code Cleanup` and enable `Run Code Cleanup profile on Save`
7. Select `Configure Code Cleanup` and add `Format document (C++)` to the profile under `Included fixers`
