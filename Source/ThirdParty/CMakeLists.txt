# ---------------------------------------------------------------------------
# Source/ThirdParty/CMakeLists.txt
#
# Every third-party library is vendored (source only, pruned) in a folder next
# to this file. This script builds them as STATIC libraries with their tests,
# samples, tools and install rules switched off, and exposes one umbrella
# target for the engine to link:
#
#     BlueJay::ThirdParty
#
# See VERSIONS.md for the pinned versions and what was removed from each.
# ---------------------------------------------------------------------------
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  # Allows `cmake -S Source/ThirdParty` on its own (used to check this file).
  cmake_minimum_required(VERSION 3.25)
  project(BlueJayThirdParty LANGUAGES C CXX)
else()
  enable_language(C)
endif()

# Several vendored projects still declare cmake_minimum_required(< 3.5).
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

option(BLUEJAY_ENABLE_PROFILING "Compile Tracy instrumentation in (TRACY_ENABLE)" ON)
set(BLUEJAY_ENABLE_NETWORKING "AUTO" CACHE STRING
  "Build GameNetworkingSockets (needs Protobuf; OpenSSL on non-Windows): AUTO, ON or OFF")
set_property(CACHE BLUEJAY_ENABLE_NETWORKING PROPERTY STRINGS AUTO ON OFF)
option(BLUEJAY_IMGUI_BACKEND_VULKAN "Compile the Dear ImGui Vulkan backend (needs the Vulkan headers)" OFF)
option(BLUEJAY_IMGUI_BACKEND_OPENGL3 "Compile the Dear ImGui OpenGL3 backend" OFF)
option(BLUEJAY_IMGUI_BACKEND_DX11 "Compile the Dear ImGui DirectX 11 backend (Windows)" OFF)
option(BLUEJAY_IMGUI_BACKEND_DX12 "Compile the Dear ImGui DirectX 12 backend (Windows)" OFF)

set(_TP "${CMAKE_CURRENT_LIST_DIR}")

# Libraries are static unless a library's own block says otherwise. Using a
# plain variable (not CACHE) so this does not leak to the game's own targets.
set(BUILD_SHARED_LIBS OFF)

# Helper: set a vendored project's option without it being overridden by an
# earlier cache entry, and keep it out of the cmake-gui.
macro(_tp_set _name _value)
  set(${_name} ${_value} CACHE BOOL "" FORCE)
  mark_as_advanced(${_name})
endmacro()

# Everything below is third-party code: do not let it fail on warnings.
set(_tp_targets "")
function(_tp_register)
  set(_tp_targets ${_tp_targets} ${ARGN} PARENT_SCOPE)
endfunction()

# ---------------------------------------------------------------------------
# SDL3 - windowing, input, audio, GPU
# ---------------------------------------------------------------------------
_tp_set(SDL_SHARED OFF)
_tp_set(SDL_STATIC ON)
_tp_set(SDL_TESTS OFF)
_tp_set(SDL_EXAMPLES OFF)
_tp_set(SDL_INSTALL OFF)
_tp_set(SDL_INSTALL_DOCS OFF)
_tp_set(SDL_TEST_LIBRARY OFF)
add_subdirectory("${_TP}/sdl" sdl EXCLUDE_FROM_ALL)
_tp_register(SDL3::SDL3-static)

# ---------------------------------------------------------------------------
# mimalloc - general purpose allocator
# ---------------------------------------------------------------------------
_tp_set(MI_BUILD_SHARED OFF)
_tp_set(MI_BUILD_STATIC ON)
_tp_set(MI_BUILD_OBJECT OFF)
_tp_set(MI_BUILD_TESTS OFF)
add_subdirectory("${_TP}/mimalloc" mimalloc EXCLUDE_FROM_ALL)
_tp_register(mimalloc-static)

# ---------------------------------------------------------------------------
# quill - asynchronous logging
# ---------------------------------------------------------------------------
add_subdirectory("${_TP}/quill" quill EXCLUDE_FROM_ALL)
_tp_register(quill::quill)

# ---------------------------------------------------------------------------
# Tracy - frame profiler (client only; the profiler UI is a separate download)
# ---------------------------------------------------------------------------
set(TRACY_STATIC ON CACHE BOOL "" FORCE)
set(TRACY_ENABLE ${BLUEJAY_ENABLE_PROFILING} CACHE BOOL "" FORCE)
add_subdirectory("${_TP}/tracy" tracy EXCLUDE_FROM_ALL)
_tp_register(Tracy::TracyClient)

# ---------------------------------------------------------------------------
# Crash reporting: Sentry Native, using its bundled CMake build of Crashpad
# ---------------------------------------------------------------------------
set(SENTRY_BACKEND "crashpad" CACHE STRING "" FORCE)
_tp_set(SENTRY_BUILD_SHARED_LIBS OFF)
_tp_set(SENTRY_BUILD_TESTS OFF)
_tp_set(SENTRY_BUILD_EXAMPLES OFF)
_tp_set(SENTRY_BUILD_BENCHMARKS OFF)
_tp_set(SENTRY_ENABLE_INSTALL OFF)
add_subdirectory("${_TP}/sentry-native" sentry-native EXCLUDE_FROM_ALL)
_tp_register(sentry::sentry)
# chromium/crashpad (the upstream repo, ./crashpad) is vendored as source only:
# upstream builds it with GN, so it has no CMake target. Sentry Native's own
# external/crashpad fork is the one the engine links.

# ---------------------------------------------------------------------------
# Core containers / ECS
# ---------------------------------------------------------------------------
add_subdirectory("${_TP}/entt" entt EXCLUDE_FROM_ALL)
_tp_register(EnTT::EnTT)

_tp_set(FLECS_STATIC ON)
_tp_set(FLECS_SHARED OFF)
_tp_set(FLECS_TESTS OFF)
add_subdirectory("${_TP}/flecs" flecs EXCLUDE_FROM_ALL)
_tp_register(flecs::flecs_static)

# ---------------------------------------------------------------------------
# Physics, navigation
# ---------------------------------------------------------------------------
# Jolt follows the engine's MSVC runtime instead of forcing the static one.
_tp_set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)
_tp_set(TARGET_UNIT_TESTS OFF)
_tp_set(TARGET_HELLO_WORLD OFF)
_tp_set(TARGET_PERFORMANCE_TEST OFF)
_tp_set(TARGET_SAMPLES OFF)
_tp_set(TARGET_VIEWER OFF)
_tp_set(INTERPROCEDURAL_OPTIMIZATION OFF)
add_subdirectory("${_TP}/joltphysics/Build" joltphysics EXCLUDE_FROM_ALL)
_tp_register(Jolt)

_tp_set(BOX3D_SAMPLES OFF)
_tp_set(BOX3D_UNIT_TESTS OFF)
_tp_set(BOX3D_BENCHMARKS OFF)
_tp_set(BOX3D_DOCS OFF)
add_subdirectory("${_TP}/box3d" box3d EXCLUDE_FROM_ALL)
_tp_register(box3d)

_tp_set(RECASTNAVIGATION_DEMO OFF)
_tp_set(RECASTNAVIGATION_TESTS OFF)
_tp_set(RECASTNAVIGATION_EXAMPLES OFF)
add_subdirectory("${_TP}/recastnavigation" recastnavigation EXCLUDE_FROM_ALL)
_tp_register(Recast Detour DetourCrowd DetourTileCache DebugUtils)

# ---------------------------------------------------------------------------
# Animation
# ---------------------------------------------------------------------------
_tp_set(ozz_build_tools OFF)
_tp_set(ozz_build_fbx OFF)
_tp_set(ozz_build_gltf OFF)
_tp_set(ozz_build_samples OFF)
_tp_set(ozz_build_howtos OFF)
_tp_set(ozz_build_tests OFF)
# Match the DLL CRT the rest of the project uses (ozz defaults to the static CRT, which
# fails to link with LNK2038 once engine code pulls in std stream symbols).
_tp_set(ozz_build_msvc_rt_dll ON)
add_subdirectory("${_TP}/ozz-animation" ozz-animation EXCLUDE_FROM_ALL)
_tp_register(ozz_base ozz_animation ozz_animation_offline ozz_geometry)

# ACL is header-only and depends on RTM (vendored under acl/external/rtm).
add_library(acl INTERFACE)
add_library(acl::acl ALIAS acl)
target_include_directories(acl SYSTEM INTERFACE
  "${_TP}/acl/includes"
  "${_TP}/acl/external/rtm/includes")
target_compile_features(acl INTERFACE cxx_std_11)
_tp_register(acl)

# ---------------------------------------------------------------------------
# Assets: glTF, mesh optimisation, KTX2 textures
# ---------------------------------------------------------------------------
_tp_set(MESHOPT_BUILD_DEMO OFF)
_tp_set(MESHOPT_BUILD_GLTFPACK OFF)
_tp_set(MESHOPT_INSTALL OFF)
add_subdirectory("${_TP}/meshoptimizer" meshoptimizer EXCLUDE_FROM_ALL)
_tp_register(meshoptimizer)

# fastgltf wants simdjson + simdutf. They are vendored in fastgltf/deps (same
# versions and SHA-256 as fastgltf's cmake/dependencies.cmake pins) and staged
# where fastgltf looks first, so configuring never touches the network.
set(_fg_bin "${CMAKE_CURRENT_BINARY_DIR}/fastgltf")
file(COPY "${_TP}/fastgltf/deps/simdjson/simdjson.h" "${_TP}/fastgltf/deps/simdjson/simdjson.cpp"
  DESTINATION "${_fg_bin}/simdjson")
file(COPY "${_TP}/fastgltf/deps/simdutf/simdutf.h" "${_TP}/fastgltf/deps/simdutf/simdutf.cpp"
  DESTINATION "${_fg_bin}/simdutf")
_tp_set(FASTGLTF_ENABLE_TESTS OFF)
_tp_set(FASTGLTF_ENABLE_EXAMPLES OFF)
_tp_set(FASTGLTF_ENABLE_DOCS OFF)
_tp_set(FASTGLTF_ENABLE_INSTALL OFF)
add_subdirectory("${_TP}/fastgltf" fastgltf EXCLUDE_FROM_ALL)
_tp_register(fastgltf::fastgltf)

# KTX-Software: libktx only (no CLI tools, tests or language bindings).
_tp_set(KTX_FEATURE_TOOLS OFF)
_tp_set(KTX_FEATURE_DOC OFF)
_tp_set(KTX_FEATURE_JNI OFF)
_tp_set(KTX_FEATURE_PY OFF)
_tp_set(KTX_FEATURE_TESTS OFF)
_tp_set(KTX_FEATURE_TOOLS_CTS OFF)
_tp_set(KTX_FEATURE_STATIC_LIBRARY ON)
# KTX derives its version from `git describe`; the vendored copy has no .git.
set(KTX_GIT_VERSION_FULL "v4.4.2" CACHE STRING "" FORCE)
add_subdirectory("${_TP}/ktx" ktx EXCLUDE_FROM_ALL)
_tp_register(ktx)

# ---------------------------------------------------------------------------
# Text: FreeType + HarfBuzz
# ---------------------------------------------------------------------------
_tp_set(FT_DISABLE_ZLIB ON)
_tp_set(FT_DISABLE_BZIP2 ON)
_tp_set(FT_DISABLE_PNG ON)
_tp_set(FT_DISABLE_HARFBUZZ ON)
_tp_set(FT_DISABLE_BROTLI ON)
add_subdirectory("${_TP}/freetype" freetype EXCLUDE_FROM_ALL)
_tp_register(freetype)

_tp_set(HB_BUILD_UTILS OFF)
_tp_set(HB_BUILD_GPU_DEMO OFF)
_tp_set(HB_HAVE_FREETYPE OFF)
_tp_set(HB_HAVE_GLIB OFF)
_tp_set(HB_HAVE_ICU OFF)
_tp_set(HB_HAVE_CAIRO OFF)
_tp_set(HB_HAVE_GRAPHITE2 OFF)
add_subdirectory("${_TP}/harfbuzz" harfbuzz EXCLUDE_FROM_ALL)
_tp_register(harfbuzz)

# ---------------------------------------------------------------------------
# File watching
# ---------------------------------------------------------------------------
_tp_set(BUILD_SHARED_LIBS OFF)
_tp_set(BUILD_STATIC_LIBS ON)
_tp_set(BUILD_TEST_APP OFF)
_tp_set(BUILD_AGENT OFF)
_tp_set(EFSW_INSTALL OFF)
add_subdirectory("${_TP}/efsw" efsw EXCLUDE_FROM_ALL)
_tp_register(efsw-static)

# ---------------------------------------------------------------------------
# Dear ImGui - no upstream CMake, so a target is defined here
# ---------------------------------------------------------------------------
set(_imgui "${_TP}/imgui")
add_library(imgui STATIC
  "${_imgui}/imgui.cpp"
  "${_imgui}/imgui_demo.cpp"
  "${_imgui}/imgui_draw.cpp"
  "${_imgui}/imgui_tables.cpp"
  "${_imgui}/imgui_widgets.cpp"
  "${_imgui}/misc/cpp/imgui_stdlib.cpp"
  "${_imgui}/misc/freetype/imgui_freetype.cpp"
  "${_imgui}/backends/imgui_impl_sdl3.cpp"
  "${_imgui}/backends/imgui_impl_sdlrenderer3.cpp"
  "${_imgui}/backends/imgui_impl_sdlgpu3.cpp")
add_library(imgui::imgui ALIAS imgui)
target_include_directories(imgui PUBLIC "${_imgui}" "${_imgui}/backends" "${_imgui}/misc/cpp")
target_include_directories(imgui PRIVATE "${_imgui}/misc/freetype")
target_compile_definitions(imgui PUBLIC IMGUI_ENABLE_FREETYPE)
target_link_libraries(imgui PUBLIC SDL3::SDL3-static freetype)
if(BLUEJAY_IMGUI_BACKEND_VULKAN)
  target_sources(imgui PRIVATE "${_imgui}/backends/imgui_impl_vulkan.cpp")
endif()
if(BLUEJAY_IMGUI_BACKEND_OPENGL3)
  target_sources(imgui PRIVATE "${_imgui}/backends/imgui_impl_opengl3.cpp")
endif()
if(BLUEJAY_IMGUI_BACKEND_DX11 AND WIN32)
  target_sources(imgui PRIVATE "${_imgui}/backends/imgui_impl_dx11.cpp")
  target_link_libraries(imgui PUBLIC d3d11 dxgi d3dcompiler)
endif()
if(BLUEJAY_IMGUI_BACKEND_DX12 AND WIN32)
  target_sources(imgui PRIVATE "${_imgui}/backends/imgui_impl_dx12.cpp")
  target_link_libraries(imgui PUBLIC d3d12 dxgi d3dcompiler)
endif()
_tp_register(imgui)

# ---------------------------------------------------------------------------
# Networking: GameNetworkingSockets (needs Protobuf; not vendored)
# ---------------------------------------------------------------------------
set(_net_ok FALSE)
if(NOT BLUEJAY_ENABLE_NETWORKING STREQUAL "OFF")
  find_package(Protobuf QUIET)
  if(WIN32)
    set(_gns_crypto "BCrypt")        # Windows CNG: no OpenSSL needed
    set(_gns_crypto25519 "Reference")
    set(_crypto_found TRUE)
  else()
    find_package(OpenSSL QUIET)
    set(_gns_crypto "OpenSSL")
    set(_gns_crypto25519 "OpenSSL")
    set(_crypto_found ${OPENSSL_FOUND})
  endif()
  if(Protobuf_FOUND AND _crypto_found)
    set(_net_ok TRUE)
  elseif(BLUEJAY_ENABLE_NETWORKING STREQUAL "ON")
    message(FATAL_ERROR
      "BLUEJAY_ENABLE_NETWORKING=ON but Protobuf (and OpenSSL off Windows) was not found. "
      "Install them (for example `vcpkg install protobuf`) and point CMAKE_PREFIX_PATH / "
      "CMAKE_TOOLCHAIN_FILE at them.")
  else()
    message(STATUS "GameNetworkingSockets skipped: Protobuf not found (set BLUEJAY_ENABLE_NETWORKING=ON to require it).")
  endif()
endif()
if(_net_ok)
  set(USE_CRYPTO "${_gns_crypto}" CACHE STRING "" FORCE)
  set(USE_CRYPTO25519 "${_gns_crypto25519}" CACHE STRING "" FORCE)
  _tp_set(BUILD_STATIC_LIB ON)
  _tp_set(BUILD_SHARED_LIB OFF)
  _tp_set(BUILD_EXAMPLES OFF)
  _tp_set(BUILD_TESTS OFF)
  _tp_set(BUILD_TOOLS OFF)
  _tp_set(USE_STEAMWEBRTC OFF)
  add_subdirectory("${_TP}/gamenetworkingsockets" gamenetworkingsockets EXCLUDE_FROM_ALL)
  _tp_register(GameNetworkingSockets::GameNetworkingSockets_s)
endif()

# ---------------------------------------------------------------------------
# Umbrella target
# ---------------------------------------------------------------------------
add_library(bluejay_third_party INTERFACE)
add_library(BlueJay::ThirdParty ALIAS bluejay_third_party)
target_link_libraries(bluejay_third_party INTERFACE ${_tp_targets})
message(STATUS "BlueJay third-party libraries: ${_tp_targets}")
