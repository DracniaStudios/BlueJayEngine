# --- Behaviour test -------------------------------------------------------
add_executable(bluejay_tests core/VersionTests.cpp)
target_link_libraries(bluejay_tests PRIVATE BlueJay::Engine)
add_test(NAME bluejay.core.version COMMAND bluejay_tests)

add_executable(bluejay_memory_tests core/MemoryTests.cpp)
target_link_libraries(bluejay_memory_tests PRIVATE BlueJay::Engine)
add_test(NAME bluejay.core.memory COMMAND bluejay_memory_tests)

# --- Rule: no backend headers in public headers ----------------------------
add_test(NAME bluejay.public_headers_clean
  COMMAND "${CMAKE_COMMAND}"
          -DINCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
          -P "${PROJECT_SOURCE_DIR}/cmake/CheckPublicHeaders.cmake")

# --- Rule: every public header compiles on its own -------------------------
# One generated translation unit per header, built as an OBJECT library, so a
# header with a missing include fails the build.
file(GLOB_RECURSE BLUEJAY_PUBLIC_HEADERS
  RELATIVE "${PROJECT_SOURCE_DIR}/include"
  CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/include/bluejay/*.h"
  "${PROJECT_SOURCE_DIR}/include/bluejay/*.hpp")
list(APPEND BLUEJAY_PUBLIC_HEADERS "bluejay/core/Version.h")  # generated

set(_header_check_sources "")
foreach(_header IN LISTS BLUEJAY_PUBLIC_HEADERS)
  string(MAKE_C_IDENTIFIER "${_header}" _id)
  set(_tu "${CMAKE_CURRENT_BINARY_DIR}/header_check/${_id}.cpp")
  file(GENERATE OUTPUT "${_tu}" CONTENT "#include <${_header}>\n")
  list(APPEND _header_check_sources "${_tu}")
endforeach()

add_library(bluejay_header_check OBJECT ${_header_check_sources})
target_link_libraries(bluejay_header_check PRIVATE BlueJay::Engine)
