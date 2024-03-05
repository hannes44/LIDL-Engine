# GameEngineTDA572

Setup:
1. Clone the repository with `git clone --recurse-submodules https://github.com/hannes44/GameEngineTDA572.git`
2. Install [CMake](https://cmake.org/download/)
3. Run cmake at the top folder layer. For example: `cmake -G "Visual Studio 17 2022" -A x64 -S . -B build`
4. Copy the `vendor` folder and place it inside the newly generated `build` folder, replacing the existing `build/vendor` folder
5. Go to the generated build folder and open `GameEngine.sln` in Visual Studio 2022
6. Right click either `Editor`, `Game` or `Game2` depending on what you want to run in the Solution explorer and select `Set as Startup Project`
7. Go to `Tools -> Options -> Text Editor -> Code Cleanup` and enable `Run Code Cleanup profile on Save`
8. Select `Configure Code Cleanup` and add `Format document (C++)` to the profile under `Included fixers`