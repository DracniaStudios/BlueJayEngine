cmake_minimum_required(VERSION 3.25)

project(BlueJay
  VERSION 0.1.0
  DESCRIPTION "BlueJay Engine - the engine behind Project Veil"
  # C is enabled too: several vendored libraries (SDL, mimalloc, KTX, FreeType,
  # Sentry, ...) are C, and their targets propagate C feature requirements.
  LANGUAGES C CXX)

set(PRODUCTION_BUILD OFF CACHE BOOL "Production Build" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Build")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Library")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Archive")  # static libs (.lib/.a)

# ---------------------------------------------------------------------------
# Options
#
# Defaults are chosen so that importing BlueJay from a game (add_subdirectory
# or FetchContent) adds ONLY the library: no tests, no install rules.
# ---------------------------------------------------------------------------
option(BLUEJAY_BUILD_TESTS "Build the BlueJay tests" ON)
option(BLUEJAY_BUILD_EXAMPLES "Build the example programs in examples/" ON)
option(BLUEJAY_INSTALL "Generate install rules and find_package(BlueJay) files" ${PROJECT_IS_TOP_LEVEL})
option(BLUEJAY_HEADLESS
  "Build without window/audio/input backends (tests, CI, servers). Every system must update with rendering off."
  OFF)

include(GNUInstallDirs)

# ---------------------------------------------------------------------------
# Source layout
#
# Engine code lives in Source/Runtime/<Module>/, each header next to its .cpp.
# Source/Runtime is the include root, so includes name the module:
#   #include <Core/ArenaAllocator.h>
#
# BLUEJAY_MODULES lists the modules that are part of the library so far. Only
# these are installed and covered by the header tests. Add a module here once
# its code compiles inside BlueJay.
# ---------------------------------------------------------------------------
set(BLUEJAY_RUNTIME_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source/Runtime")
set(BLUEJAY_MODULES Core)

# ---------------------------------------------------------------------------
# Generated version header (lives in the build tree, installed with the rest)
# ---------------------------------------------------------------------------
set(BLUEJAY_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
configure_file(
  Source/Version.h.in
  "${BLUEJAY_GENERATED_DIR}/Core/Version.h"
  @ONLY)

# ---------------------------------------------------------------------------
# CMake Build Optomizations
# ---------------------------------------------------------------------------

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  if (BLUEJAY_ENABLE_PROFILING)
    # Edit and Continue (/ZI) makes __LINE__ non-constant, which breaks Tracy's zone
    # macros, so profiling builds use plain /Zi debug info.
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>")
  else()
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
  endif()
endif()

if (MSVC)
	set(RUNTIME "<MultiThreaded$<$<CONFIG:Debug>:Debug>")
	set(RUNTIME "<MultiThreaded$<$<CONFIG:Release>:Release>")
	#add_compile_options(arch/AVX2)
else()
	#add_compile_options(0mavx2 -CMakeFindDependencyMacro)
endif()

include (CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
if (ipo_supported)
	message (STATUS "IPO / LTO enabled")
	if (PRODUCTION_BUILD)
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)# LINK time optimization
	else()
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
	endif()
else()
	message(WARNING "IPO / LTO not supported: ${ipo_error}")
endif()
# ---------------------------------------------------------------------------
# The engine library. One target for now; the folder layout already mirrors
# the per-module split (see docs/ARCHITECTURE.md).
# ---------------------------------------------------------------------------

# Recursively glob every module under Source/Runtime.
file(GLOB_RECURSE BLUEJAY_RUNTIME_SOURCES CONFIGURE_DEPENDS
  "${BLUEJAY_RUNTIME_DIR}/*.cpp"
  "${BLUEJAY_RUNTIME_DIR}/*.h"
  "${BLUEJAY_RUNTIME_DIR}/*.hpp")

add_library(bluejay_engine STATIC
  Source/Version.cpp
  ${BLUEJAY_RUNTIME_SOURCES})
add_library(BlueJay::Engine ALIAS bluejay_engine)

set_target_properties(bluejay_engine PROPERTIES
  OUTPUT_NAME bluejay
  EXPORT_NAME Engine
  CXX_EXTENSIONS OFF)

target_compile_features(bluejay_engine PUBLIC cxx_std_23)

target_include_directories(bluejay_engine
  PUBLIC
    "$<BUILD_INTERFACE:${BLUEJAY_RUNTIME_DIR}>"
    "$<BUILD_INTERFACE:${BLUEJAY_GENERATED_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

if(MSVC)
  target_compile_options(bluejay_engine PRIVATE /W4 /permissive-)
else()
  target_compile_options(bluejay_engine PRIVATE -Wall -Wextra)
endif()

if(BLUEJAY_HEADLESS)
  target_compile_definitions(bluejay_engine PUBLIC BLUEJAY_HEADLESS=1)
endif()

# ---------------------------------------------------------------------------
# Third-party libraries (vendored under Source/ThirdParty, see VERSIONS.md).
#
# Linked PRIVATE: backend types stay out of public headers (see
# cmake/CheckPublicHeaders.cmake). Wrapped in BUILD_INTERFACE because the
# vendored libraries have no install/export rules of their own; an installed
# package therefore does not carry them yet. When a backend becomes part of the
# public link interface, add the matching find_dependency() to
# cmake/BlueJayConfig.cmake.in.
# ---------------------------------------------------------------------------
add_subdirectory(Source/ThirdParty)
target_link_libraries(bluejay_engine PRIVATE "$<BUILD_INTERFACE:BlueJay::ThirdParty>")

# ---------------------------------------------------------------------------
# Install + find_package(BlueJay) support
# ---------------------------------------------------------------------------
if(BLUEJAY_INSTALL)
  include(CMakePackageConfigHelpers)

  set(BLUEJAY_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/BlueJay")

  install(TARGETS bluejay_engine
    EXPORT BlueJayTargets
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

  foreach(_module IN LISTS BLUEJAY_MODULES)
    install(DIRECTORY "${BLUEJAY_RUNTIME_DIR}/${_module}/"
      DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${_module}"
      FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
  endforeach()

  install(FILES "${BLUEJAY_RUNTIME_DIR}/BlueJay.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

  install(FILES "${BLUEJAY_GENERATED_DIR}/Core/Version.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Core")

  install(EXPORT BlueJayTargets
    FILE BlueJayTargets.cmake
    NAMESPACE BlueJay::
    DESTINATION "${BLUEJAY_CMAKE_INSTALL_DIR}")

  # Pre-1.0: a minor bump may break the API, so only the same minor matches.
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/BlueJayConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion)

  configure_package_config_file(
    cmake/BlueJayConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/BlueJayConfig.cmake"
    INSTALL_DESTINATION "${BLUEJAY_CMAKE_INSTALL_DIR}")

  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/BlueJayConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/BlueJayConfigVersion.cmake"
    DESTINATION "${BLUEJAY_CMAKE_INSTALL_DIR}")
endif()

# ---------------------------------------------------------------------------
# Examples (opt-in, they open windows)
# ---------------------------------------------------------------------------
if(BLUEJAY_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
if(BLUEJAY_BUILD_TESTS)
  enable_testing()
  add_subdirectory(Source/Tests)
endif()
