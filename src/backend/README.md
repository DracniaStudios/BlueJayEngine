# src/backend

The only place in the engine where third-party headers may be included.

Public headers in `include/bluejay/` describe what a system does using engine types and opaque handles. The code here implements it with a specific library. Swapping a library (raylib to SDL3 before 1.0) is then a change inside one folder here, with no change to any public header or to the game.

| Folder | Library | Notes |
|---|---|---|
| `raylib/` | raylib | Current window, input, rendering |
| `sdl3/` | SDL3 + OpenGL | Reserved for the pre-1.0 swap, time-boxed with raylib as the fallback |
| `fmod/` | FMOD | Audio |
| `entt/` | EnTT | Sits behind the Entity API |
| `gns/` | GameNetworkingSockets | Network transport |

The `bluejay.public_headers_clean` test fails if a header under `include/` includes any of these libraries.
