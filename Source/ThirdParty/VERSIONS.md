# Third-party libraries

Vendored, source-only, pinned to a release tag. No `.git` metadata and no submodules
are kept. Everything is built from `CMakeLists.txt` in this folder; the engine links
the umbrella target `BlueJay::ThirdParty`.

| Folder | Upstream | Pinned | Role in the engine |
| --- | --- | --- | --- |
| `acl` | nfrechette/acl | v2.1.0 | Animation compression (header-only; RTM math lib vendored in `acl/external/rtm`) |
| `box3d` | erincatto/box3d | v0.1.0 | 3D physics (alpha upstream) |
| `crashpad` | chromium/crashpad | `f3769d9` (no tags upstream) | Crash reporter, **source only** - upstream builds with GN. `third_party/mini_chromium` pinned at `c029983`. The engine links Sentry Native's CMake-buildable Crashpad fork instead |
| `efsw` | SpartanJ/efsw | 1.7.3 | File watching for hot reload |
| `entt` | skypjack/entt | v4.0.0 | ECS |
| `fastgltf` | spnda/fastgltf | v1.0.1 | glTF import. `deps/` holds simdjson 5.1.0 and simdutf 9.3.3 (the exact files and SHA-256 fastgltf's CMake would download) |
| `flecs` | SanderMertens/flecs | v4.2.0 | ECS |
| `freetype` | freetype/freetype | VER-2-15-0 | Font rasterisation |
| `gamenetworkingsockets` | ValveSoftware/GameNetworkingSockets | v1.6.1 | Networking. Needs **Protobuf** (not vendored); OpenSSL off Windows (Windows uses CNG/BCrypt) |
| `harfbuzz` | harfbuzz/harfbuzz | 15.2.0 | Text shaping |
| `imgui` | ocornut/imgui | v1.93.0 | Debug/editor UI. Target defined in `CMakeLists.txt` (upstream has none) |
| `joltphysics` | jrouwe/JoltPhysics | v5.7.0 | 3D physics |
| `ktx` | KhronosGroup/KTX-Software | v4.4.2 | KTX2 textures (libktx only) |
| `meshoptimizer` | zeux/meshoptimizer | v1.3 | Mesh cooking |
| `mimalloc` | microsoft/mimalloc | v3.7.0 | Allocator |
| `ozz-animation` | guillaumeblanc/ozz-animation | 0.17.0 | Skeletal animation runtime + offline library |
| `quill` | odygrd/quill | v13.1.0 | Logging |
| `recastnavigation` | recastnavigation/recastnavigation | v1.6.0 | Navmesh |
| `sdl` | libsdl-org/SDL | release-3.4.20 | Platform layer (SDL3) |
| `sentry-native` | getsentry/sentry-native | 0.17.6 | Crash reporting (Crashpad backend) |
| `tracy` | wolfpld/tracy | v0.15.0 | Profiler client (the profiler UI is a separate download) |

## Differences from `party.txt`

* `Kaelio/ktx` is an unrelated AI data tool; the texture library is `KhronosGroup/KTX-Software`.
* `getsentry/sentry` is the 1 GB Sentry server; the C/C++ SDK is `getsentry/sentry-native`.

## What was removed

Per repository: tests, samples/examples, benchmarks, documentation sources, CI and
editor config, alternate build systems (Bazel, Meson, Make, premake, MSBuild), tools
and demos, and platform trees for builds the engine does not target. License files
are kept in every folder.

Notable specifics:

* `gamenetworkingsockets`: `src/external/webrtc` and `abseil` (only for `USE_STEAMWEBRTC`, off) removed; native ICE stays.
* `sentry-native`: Breakpad, libunwindstack-ndk (Android) and `external/CMakeLists.txt` (Breakpad only) removed. Use `SENTRY_BACKEND=crashpad`.
* `imgui`: only the SDL3, SDL_Renderer3, SDL_GPU3, Vulkan, OpenGL3, DX11 and DX12 backends kept.
* `ktx`: CLI tools, tests, Java/Python bindings and their dependencies (fmt, cxxopts) removed. `utils/` and `scripts/mkversion` are kept because the library build needs them. The build writes `lib/version.h` into the source tree; it is git-ignored.
* `harfbuzz`: `meson.build` is kept because HarfBuzz's CMake reads its version from it.
* `meshoptimizer`: `gltfpack` removed.
* `ozz-animation`: tools, glTF/FBX importers, samples removed (offline *library* is kept).

## Local patches to upstream files

* `ktx/CMakeLists.txt`: removed the unconditional `add_subdirectory(tests)`.

Anything else in this folder is unmodified upstream source. Nested `.gitignore` files were removed because they hid required files (for example `astc-encoder/CMakeLists.txt`), and the root `.gitignore` re-includes `Source/ThirdParty/**` so the few shipped binaries (mimalloc redirect DLLs, crashpad `.obj`) are tracked.
