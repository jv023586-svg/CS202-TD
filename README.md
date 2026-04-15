# CS202-TD
Lab project for CS202. Tower Defense game.

## Requirements
- CMake 3.22 or newer
- A C++17 compiler
- Git

## Build and Run (Windows PowerShell)
From the repository root:

```powershell
cmake -S . -B build
cmake --build build --config Debug
.\build\bin\Debug\main.exe
```

## Build and Run (macOS/Linux)
From the repository root:

```bash
cmake -S . -B build
cmake --build build
./build/bin/main
```

## Notes
- The executable is generated in the `build` folder on purpose.
- CMake automatically copies `assets` next to the executable after each build.
- Run the executable from the generated `build/bin/...` path shown above.
- SFML is embedded into this repository and this feature may not be compatible with all Operating Systems. 
