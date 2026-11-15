# Migration map: legacy root folders to the new layout

Status as of 2026-11-01: **step 1 done (`Memory/`). Everything else is waiting on the three decisions below.**

The folders still at the repo root (`Assets/`, `Audio/`, `Editor/`, ...) are exactly as you left them, except `Memory/`, which moved. This map comes from a file scan (line counts, `#include` lines, a search for Veil type names) and from reading `Settings.cpp` in full. Only `Memory/` has been compiled and tested.

## READ-ONLY: Done

| What | Result |
|---|---|
| `Memory/ArenaAllocator.h` | Now `include/bluejay/core/ArenaAllocator.h`. `Arena` is now `bluejay::memory::Arena` |
| `Memory/PoolAllocator.h` | Now `include/bluejay/core/PoolAllocator.h`. `MemoryAllocator::PoolAllocator` is now `bluejay::memory::PoolAllocator` |
| Changes inside the headers | Include guards replaced with `#pragma once`; `#include <new>` added to Arena (it uses `std::bad_alloc`); namespace changed. Nothing else. CRLF line endings kept |
| Test | `tests/core/MemoryTests.cpp` (`bluejay.core.memory`), including a regression test for the pool slot-stride bug the header's comments describe. Passes on g++ and clang, and under AddressSanitizer + UBSan on g++. Three deliberately broken allocators all make it fail |
| Originals | Archived untouched in `docs/archive/legacy/Memory/` |

Nothing else in the legacy tree used these allocators. **Veil may.** If Veil's own code names `Arena` or `MemoryAllocator::PoolAllocator`, those references need the new names when Veil adopts the engine's copy. I could not see Veil.

## READ-ONLY: What the scan found

- About 5.2k lines outside `Editor/`, plus 18 files in `Editor/`.
- Includes between these files are flat and local (`#include "Scene.h"`), with no `../` paths, so a move needs include rewrites for the public headers only.
- Third-party use: raylib (`raylib.h` in 10 includes, plus `raymath.h` and `rlgl.h`), nlohmann/json (`SaveStore.h` and one other file), and imgui/rlImGui plus `UIEngine.h` in `Editor/`. `UIEngine.h` is not in this folder.
- The word-search for Veil types has false positives. "Directory" matched `Director`, and "Player" in `Lighting` is naming, not a type dependency.

## READ-ONLY: Correction: `Settings` is not a clean move

I first listed `Settings` as engine code with no third-party dependency. That was wrong. After reading all of `Settings.cpp`, it is three things in one file:

| Part | Where | Engine or game |
|---|---|---|
| **Mechanism**: `Kind`, `Descriptor`, `FormatValue`, validation, `Set` / `Cycle` / `Subscribe` / `Notify` | `Settings.h`, most of `Settings.cpp` | Engine. No third-party dependency |
| **Persistence**: `Load` / `Save` through `SaveStore` | `Settings.cpp` | Engine in intent, but `SaveStore.h` puts `nlohmann::json` in its public interface, which the header rule forbids |
| **Game content and wiring**: `GameDescriptors()` (Difficulty, HideClueVFX, Shadows, Luminosity, ...) and the `getInstance()` singleton with its `saves/` path | `Settings.cpp` | Game. Stays in Veil |

Callers: `Audio/AudioManager.cpp` and `Lighting/LightingSystem.cpp` call `Settings::Settings::getInstance()` directly, and `SceneManagement/Scene.h` includes it. When `Audio` and `Lighting` move, they must receive their settings by injection, because the engine cannot call a Veil singleton. `Settings.cpp` also includes `<raylib.h>`, which looks unused in the code I read.

## READ-ONLY: Verdicts

| Legacy | Lines | Verdict | Goes to | Status / blocker |
|---|---|---|---|---|
| `Memory/` | 160 | Engine, clean | `core/` | **Done** |
| `Settings.h/.cpp` | 486 | **Split** (see above) | mechanism to `core/`; content and singleton stay in Veil | Blocked: D2, D3 |
| `Save/SaveStore` | 154 | Engine | `core/` | Blocked: D2 (JSON in its header) |
| `ParticleSystem/` | 81 | Engine | `render/` | Blocked: D1 (raylib) |
| `InputSystem/` | 202 | Engine | `input/` + `src/backend/raylib/` | Blocked: D1 |
| `Assets/` | 255 | Engine | `core/` or `render/` + raylib backend | Blocked: D1 |
| `Physics3D/` | 581 | Engine | `physics/` | Blocked: D1 (verify when its turn comes) |
| `Lighting/` | 1287 | Engine | `render/` | Blocked: D1, D3 (reads Settings) |
| `Audio/` | 530 | Engine | `audio/` + `src/backend/fmod/` | Blocked: D1, D3, plus the FMOD dependency |
| `Save/SaveSystem` | 487 | **Game** | stays in Veil | Writes `scene.player`, rebuilds a Stalker through `createByKind` |
| `SceneManagement/` | 869 | **Engine in intent, coupled** | `scene/` after decoupling | `Scene.cpp` does `new Player` and owns the map, sound field and station noise. Blocked by the Entity API (ECS checkpoint ticket) |
| `Editor/` | 18 files | **Game for now** | stays in Veil, or a later `BlueJay::Editor` | 11 of 18 files reference Veil types. Depends on imgui and rlImGui |
| `randomStuff.h/.cpp` | 117 | Unsorted | review by hand | |

## READ-ONLY: The three decisions that unblock everything

**D1. How does raylib enter the engine?** Almost every remaining module needs it. *Recommended:* the engine owns it through `FetchContent` at a pinned tag, and Veil stops carrying its own copy. Two raylib builds in one link will collide. The alternative is the engine finding a raylib that the game provides, which pushes the version question back onto Veil.

**D2. Where does JSON live?** `SaveStore.h` exposes `nlohmann::json` in its interface. *Recommended:* the store exchanges serialized text (`std::string`), and nlohmann is included only in `.cpp` files, so the public-header test stays green and the engine takes one header-only dependency.

**D3. How is `Settings` split?** *Recommended:* the mechanism moves to the engine; `GameDescriptors()` and the singleton stay in Veil; `Audio` and `Lighting` take a settings reference at construction instead of calling `getInstance()`.

## READ-ONLY: Order from here

1. **Done:** `Memory/`.
2. After D1: `ParticleSystem`, `InputSystem`, `Assets`, `Physics3D`. Each is verified when its turn comes.
3. After D2 and D3: `SaveStore`, the `Settings` mechanism, then `Lighting` and `Audio`.
4. `Scene`, `SceneManager` come with the Entity API.
5. `Editor/` is decided after ECS lands.

Rule per file: one commit, and `cmake --build` plus `ctest` green before the next. A raylib or FMOD include in a public header fails `bluejay.public_headers_clean`.

## READ-ONLY: Repo hygiene I found (git was not touched)

- `origin` is `https://github.com/thebestnoob1129/Terraria.git`. Do not push this folder there.
- About 1,285 files from the old Terraria template (`thirdparty/`, `src/`, `resources/`) are staged as added but are missing on disk. Two tracked files (`.gitignore`, `README.md`) are deleted on disk, and the new skeleton recreates them with different contents.
- The old root `CMakeLists.txt` was replaced. A copy is in `docs/archive/CMakeLists.terraria.txt`, and the original is still in git HEAD and the index.
- `docs/archive/` holds old code on purpose. Delete it, or leave it out of the first commit, once you have reviewed it.

## STEP 1: RUN (PowerShell) when you are ready to clean the index

Review the output of the first status command before running `git add -A`.

```powershell
cd D:\GitHub\BlueJayEngine
git status --short | Select-Object -First 30
git add -A
git status --short | Select-Object -First 30
```

`git add -A` turns the staged-then-deleted template files into plain deletions. It also stages the new skeleton and the legacy folders, so check the list first.
