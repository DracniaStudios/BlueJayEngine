cmake_minimum_required(VERSION 3.25)

project(BlueJay
  VERSION 0.1.0
  DESCRIPTION "BlueJay Engine - the engine behind Project Veil"
  LANGUAGES CXX)

# ---------------------------------------------------------------------------
# Options
#
# Defaults are chosen so that importing BlueJay from a game (add_subdirectory
# or FetchContent) adds ONLY the library: no tests, no install rules.
# ---------------------------------------------------------------------------
option(BLUEJAY_BUILD_TESTS "Build the BlueJay tests" ${PROJECT_IS_TOP_LEVEL})
option(BLUEJAY_INSTALL "Generate install rules and find_package(BlueJay) files" ${PROJECT_IS_TOP_LEVEL})
option(BLUEJAY_HEADLESS
  "Build without window/audio/input backends (tests, CI, servers). Every system must update with rendering off."
  OFF)

include(GNUInstallDirs)

# ---------------------------------------------------------------------------
# Generated version header (lives in the build tree, installed with the rest)
# ---------------------------------------------------------------------------
set(BLUEJAY_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
configure_file(
  include/bluejay/core/Version.h.in
  "${BLUEJAY_GENERATED_DIR}/bluejay/core/Version.h"
  @ONLY)

# ---------------------------------------------------------------------------
# The engine library. One target for now; the folder layout already mirrors
# the per-module split (see docs/ARCHITECTURE.md).
# ---------------------------------------------------------------------------
add_library(bluejay_engine STATIC
  src/core/Version.cpp
)
add_library(BlueJay::Engine ALIAS bluejay_engine)

set_target_properties(bluejay_engine PROPERTIES
  OUTPUT_NAME bluejay
  EXPORT_NAME Engine
  CXX_EXTENSIONS OFF)

target_compile_features(bluejay_engine PUBLIC cxx_std_23)

target_include_directories(bluejay_engine
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${BLUEJAY_GENERATED_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(MSVC)
  target_compile_options(bluejay_engine PRIVATE /W4 /permissive-)
else()
  target_compile_options(bluejay_engine PRIVATE -Wall -Wextra)
endif()

if(BLUEJAY_HEADLESS)
  target_compile_definitions(bluejay_engine PUBLIC BLUEJAY_HEADLESS=1)
endif()

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

  install(DIRECTORY include/
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

  install(FILES "${BLUEJAY_GENERATED_DIR}/bluejay/core/Version.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/bluejay/core")

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
# Tests
# ---------------------------------------------------------------------------
if(BLUEJAY_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
