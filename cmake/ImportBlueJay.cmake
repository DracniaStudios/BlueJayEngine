# ImportBlueJay.cmake
#
# Drop-in module for the GAME repo (Project Veil). Copy it to
#   <Veil>/cmake/ImportBlueJay.cmake
# then, in Veil's root CMakeLists.txt, AFTER project() and AFTER any
# CMAKE_MSVC_RUNTIME_LIBRARY / BUILD_SHARED_LIBS settings:
#
#   include(cmake/ImportBlueJay.cmake)
#   target_link_libraries(<your-exe-target> PRIVATE BlueJay::Engine)
#
# Choose how the engine is found with -DBLUEJAY_SOURCE=<mode>:
#
#   LOCAL    A checkout on disk, built as part of the game's own build.
#            Default: a sibling folder, <Veil>/../BlueJayEngine.
#            Override with -DBLUEJAY_LOCAL_DIR=<path>.
#   GIT      FetchContent from BLUEJAY_GIT_URL at BLUEJAY_GIT_TAG (pinned).
#            For Docker and CI, where the sibling folder does not exist.
#            Local override while developing:
#              -DFETCHCONTENT_SOURCE_DIR_BLUEJAY=<path to a checkout>
#   PACKAGE  A prebuilt install tree, found with find_package(BlueJay CONFIG).
#            Point CMAKE_PREFIX_PATH (or BlueJay_DIR) at it.
#
# In every mode the game sees exactly one thing: the BlueJay::Engine target.

include_guard(GLOBAL)

set(BLUEJAY_SOURCE "LOCAL" CACHE STRING
  "How the game gets BlueJay: LOCAL (checkout on disk), GIT (pinned tag), PACKAGE (installed)")
set_property(CACHE BLUEJAY_SOURCE PROPERTY STRINGS LOCAL GIT PACKAGE)

set(BLUEJAY_LOCAL_DIR "${CMAKE_SOURCE_DIR}/../BlueJayEngine" CACHE PATH
  "LOCAL mode: path to a BlueJay checkout")
set(BLUEJAY_GIT_URL "" CACHE STRING
  "GIT mode: repository URL of BlueJay")
set(BLUEJAY_GIT_TAG "v0.1.0" CACHE STRING
  "GIT mode: tag to fetch (use a tag, not a branch, so builds are reproducible)")
set(BLUEJAY_MIN_VERSION "0.1" CACHE STRING
  "PACKAGE mode: minimum acceptable BlueJay version")

if(BLUEJAY_SOURCE STREQUAL "LOCAL")
  get_filename_component(_bluejay_dir "${BLUEJAY_LOCAL_DIR}" ABSOLUTE)
  if(NOT EXISTS "${_bluejay_dir}/CMakeLists.txt")
    message(FATAL_ERROR
      "BLUEJAY_SOURCE=LOCAL but no BlueJay checkout at:\n  ${_bluejay_dir}\n"
      "Clone it there, or pass -DBLUEJAY_LOCAL_DIR=<path>, "
      "or use -DBLUEJAY_SOURCE=GIT / PACKAGE.")
  endif()
  message(STATUS "BlueJay: LOCAL checkout at ${_bluejay_dir}")
  add_subdirectory("${_bluejay_dir}" "${CMAKE_BINARY_DIR}/_deps/bluejay-build")

elseif(BLUEJAY_SOURCE STREQUAL "GIT")
  if(BLUEJAY_GIT_URL STREQUAL "" AND NOT FETCHCONTENT_SOURCE_DIR_BLUEJAY)
    message(FATAL_ERROR
      "BLUEJAY_SOURCE=GIT needs -DBLUEJAY_GIT_URL=<repo url> "
      "(or -DFETCHCONTENT_SOURCE_DIR_BLUEJAY=<local checkout> to skip the fetch).")
  endif()
  message(STATUS "BlueJay: GIT ${BLUEJAY_GIT_URL} @ ${BLUEJAY_GIT_TAG}")
  include(FetchContent)
  FetchContent_Declare(BlueJay
    GIT_REPOSITORY "${BLUEJAY_GIT_URL}"
    GIT_TAG        "${BLUEJAY_GIT_TAG}")
  FetchContent_MakeAvailable(BlueJay)

elseif(BLUEJAY_SOURCE STREQUAL "PACKAGE")
  message(STATUS "BlueJay: PACKAGE via find_package")
  find_package(BlueJay ${BLUEJAY_MIN_VERSION} REQUIRED CONFIG)

else()
  message(FATAL_ERROR
    "BLUEJAY_SOURCE must be LOCAL, GIT or PACKAGE (got '${BLUEJAY_SOURCE}').")
endif()

if(NOT TARGET BlueJay::Engine)
  message(FATAL_ERROR "BlueJay imported (${BLUEJAY_SOURCE}) but target BlueJay::Engine does not exist.")
endif()
