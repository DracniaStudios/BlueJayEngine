# BlueJay Architecture

## READ-ONLY: Layout

```
BlueJayEngine/
├─ CMakeLists.txt              # target BlueJay::Engine + install/export
├─ CMakePresets.json
├─ cmake/
│  ├─ BlueJayConfig.cmake.in   # find_package(BlueJay) support
│  ├─ ImportBlueJay.cmake      # drop-in for the game repo (copy it there)
│  └─ CheckPublicHeaders.cmake # test: no backend headers in public headers
├─ include/bluejay/            # PUBLIC headers only
│  ├─ bluejay.h                # umbrella header
│  ├─ core/                    # Version, Arena + PoolAllocator; Settings: see MIGRATION.md
│  └─ ecs/ scene/ input/ audio/ render/ physics/ net/    (empty, reserved)
├─ src/                        # PRIVATE: .cpp files and private headers
│  ├─ core/ ecs/ scene/ input/ audio/ render/ physics/ net/
│  └─ backend/                 # the ONLY place third-party headers are included
│     └─ raylib/ sdl3/ fmod/ entt/ gns/
├─ resources/                  # engine-owned defaults (debug font, shaders)
├─ tests/                      # run with rendering off
├─ examples/consumer/          # a game-shaped project that imports the engine
└─ docs/
```

## READ-ONLY: The rules

1. **Public vs private.** Only `include/bluejay/` is public. Games write `#include <bluejay/scene/Scene.h>`, never a relative path, and never anything under `src/`.
2. **No backend types in public headers.** raylib, SDL3, FMOD, EnTT, GameNetworkingSockets, imgui and nlohmann never appear in an `include/` header. Use opaque handles or PIMPL. Third-party includes go in `.cpp` files, and the code that talks to them lives in `src/backend/<lib>/`. This is what makes "EnTT behind the Entity API" and the later raylib to SDL3 swap a backend-folder change.
3. **One-way dependency.** The engine never knows the game exists. The separate repo enforces this by construction.
4. **Target-based CMake.** Use `target_*` commands only, with options prefixed `BLUEJAY_`. No global `include_directories`, no global flags.
5. **Assets.** The engine ships only its own `resources/`. The game passes its asset root in at init, so nothing is hard-coded relative to the executable.
6. **One library for now.** `BlueJay::Engine` is a single static library. Split into per-module targets only when a consumer needs a subset. The folders already mirror the future split.
7. **Static library, always.** The target is `STATIC` regardless of `BUILD_SHARED_LIBS`, so there are no C++ export macros or ABI worries before 1.0.
8. **Headless is a standing rule.** Every system updates with rendering off. `-DBLUEJAY_HEADLESS=ON` defines `BLUEJAY_HEADLESS=1` for the engine and its consumers; backends will honor it as they land.

## READ-ONLY: Placement test

> Could a second game use this file unchanged?

- Yes: it belongs in `engine/` (this repo).
- No: it belongs in the game.

A file that is generic in intent but holds game types (for example a `Scene` that creates a `Player`) does not pass yet. The coupling is removed through the Entity API (ECS checkpoint), then the file moves.

## READ-ONLY: What the tests enforce

| Test | Rule it guards |
|---|---|
| `bluejay.core.version` | The linked library and the headers come from the same build |
| `bluejay.core.memory` | `Arena` and `PoolAllocator` behave: alignment, no overlap, reuse, exhaustion |
| `bluejay.public_headers_clean` | Rule 2: no backend include in any public header |
| `bluejay_header_check` (builds with the tests) | Every public header compiles on its own |

## STEP 1: CHECKLIST: adding a module file

Do these in order. Each file is one commit, and the build and `ctest` must be green before the next.

1. Header goes to `include/bluejay/<module>/Name.h`. Its includes use `<bluejay/...>`. Namespace is `bluejay`, or `bluejay::<module>` when the class name is generic (for example `bluejay::memory::Arena`).
2. Source goes to `src/<module>/Name.cpp`. Add it to `add_library(bluejay_engine ...)` in `CMakeLists.txt`.
3. If it needs a third-party library, the code touching that library goes in `src/backend/<lib>/`. Add `find_dependency(<lib>)` to `cmake/BlueJayConfig.cmake.in` if the library becomes part of the public link interface.
4. Add or extend a test under `tests/`, then run:

```
cmake --build out/build/dev
ctest --test-dir out/build/dev --output-on-failure
```
