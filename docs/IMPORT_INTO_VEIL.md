# Importing BlueJay into Project Veil

## READ-ONLY: What Veil sees

Veil links exactly one thing, the target `BlueJay::Engine`, and includes `<bluejay/...>` headers. One switch picks how the engine is found: `-DBLUEJAY_SOURCE=LOCAL|GIT|PACKAGE`.

| Mode | The engine comes from | Use it for | Cost |
|---|---|---|---|
| `LOCAL` (default) | A checkout next to Veil: `D:\GitHub\BlueJayEngine` | Daily work on your PC. Edit the engine, rebuild Veil, see the change | Not available inside Docker/CI, where the sibling folder does not exist |
| `GIT` | A pinned tag, fetched at configure time | Docker, CI, a teammate's machine | Needs a reachable repo URL. Rebuilds the engine inside each consumer |
| `PACKAGE` | A prebuilt install tree via `find_package(BlueJay CONFIG)` | Later, only if build time hurts | You manage a binary matrix (compiler × Debug/Release) |

`GIT` mode can be pointed at a local checkout without a URL: `-DFETCHCONTENT_SOURCE_DIR_BLUEJAY=D:/GitHub/BlueJayEngine`. That gives you one code path for both local development and CI.

All three modes were built and run against `examples/consumer` (Linux, g++ 13 and clang). The MSVC path has not been built.

## STEP 1: COPY the module into Veil (PowerShell)

```powershell
New-Item -ItemType Directory -Force D:\GitHub\ProjectVeil\cmake | Out-Null
Copy-Item D:\GitHub\BlueJayEngine\cmake\ImportBlueJay.cmake D:\GitHub\ProjectVeil\cmake\ImportBlueJay.cmake
```

## STEP 2: PASTE INTO Veil's root CMakeLists.txt

Place it after `project(...)` and after any `CMAKE_MSVC_RUNTIME_LIBRARY` / `BUILD_SHARED_LIBS` settings, so the engine is compiled with the same C runtime as the game.

```cmake
include(cmake/ImportBlueJay.cmake)

# Replace <your-exe-target> with Veil's executable target.
target_link_libraries(<your-exe-target> PRIVATE BlueJay::Engine)
```

## STEP 3: PASTE INTO any Veil source file to prove the link

```cpp
#include <bluejay/bluejay.h>
// ...
std::printf("BlueJay %s\n", bluejay::VersionString());
```

Reconfigure and build Veil. Expected: the CMake log shows `BlueJay: LOCAL checkout at ...` and the program prints `BlueJay 0.1.0`. Remove the line once you have seen it.

## STEP 4: GIT mode for Docker/CI (do this once a remote exists)

`origin` in `D:\GitHub\BlueJayEngine` currently points at `thebestnoob1129/Terraria`. Point it at the real BlueJayEngine repo first (you run these, after committing the skeleton):

```powershell
cd D:\GitHub\BlueJayEngine
git remote set-url origin <BlueJayEngine repo url>
git tag v0.1.0
git push origin master --tags
```

Then configure Veil in the container with:

```
cmake -S . -B build -DBLUEJAY_SOURCE=GIT -DBLUEJAY_GIT_URL=<BlueJayEngine repo url> -DBLUEJAY_GIT_TAG=v0.1.0
```

Use tags, not branches, so every build is reproducible. Bumping the engine for Veil is then a one-line tag change.

## STEP 5: PACKAGE mode (optional, later)

```
cmake -S D:/GitHub/BlueJayEngine -B out/build/pkg -DCMAKE_BUILD_TYPE=Release
cmake --build out/build/pkg
cmake --install out/build/pkg --prefix D:/BlueJay-install
```

```
cmake -S . -B build -DBLUEJAY_SOURCE=PACKAGE -DCMAKE_PREFIX_PATH=D:/BlueJay-install
```

Package versions match on the same minor only (0.1.x). That is deliberate: until 1.0 a minor bump may break the API.

## READ-ONLY: Gotchas

- **C runtime.** In `LOCAL` and `GIT` mode the engine inherits Veil's compiler flags. Set `CMAKE_MSVC_RUNTIME_LIBRARY` before the include, never after, or you get `LNK2038` mismatches.
- **C++23 propagates.** `BlueJay::Engine` requires `cxx_std_23`, so Veil's target is compiled with at least that.
- **Include style.** Only `<bluejay/...>`. Anything under the engine's `src/` is private and is not on Veil's include path.
- **Nothing extra gets built.** An imported engine adds no tests and no install rules to Veil's build. Both were checked.
- **Dependencies.** The skeleton has none. When the raylib backend lands, raylib comes in through the engine's CMake, so Veil should stop adding its own copy.
