# --- Behaviour test -------------------------------------------------------
add_executable(bluejay_tests core/VersionTests.cpp)
target_link_libraries(bluejay_tests PRIVATE BlueJay::Engine)
add_test(NAME bluejay.core.version COMMAND bluejay_tests)

add_executable(bluejay_memory_tests core/MemoryTests.cpp)
target_link_libraries(bluejay_memory_tests PRIVATE BlueJay::Engine)
add_test(NAME bluejay.core.memory COMMAND bluejay_memory_tests)

# Both rules below cover only the modules in BLUEJAY_MODULES (see the root
# CMakeLists.txt), i.e. the parts of Source/Runtime that are in the library.

# --- Rule: no backend headers in public headers ----------------------------
foreach(_module IN LISTS BLUEJAY_MODULES)
  add_test(NAME bluejay.public_headers_clean.${_module}
    COMMAND "${CMAKE_COMMAND}"
            -DINCLUDE_DIR=${BLUEJAY_RUNTIME_DIR}/${_module}
            -P "${PROJECT_SOURCE_DIR}/cmake/CheckPublicHeaders.cmake")
endforeach()

# --- Rule: every public header compiles on its own -------------------------
# One generated translation unit per header, built as an OBJECT library, so a
# header with a missing include fails the build.
set(BLUEJAY_PUBLIC_HEADERS
  "BlueJay.h"        # umbrella
  "Core/Version.h")  # generated
foreach(_module IN LISTS BLUEJAY_MODULES)
  file(GLOB_RECURSE _module_headers
    RELATIVE "${BLUEJAY_RUNTIME_DIR}"
    CONFIGURE_DEPENDS
    "${BLUEJAY_RUNTIME_DIR}/${_module}/*.h"
    "${BLUEJAY_RUNTIME_DIR}/${_module}/*.hpp")
  list(APPEND BLUEJAY_PUBLIC_HEADERS ${_module_headers})
endforeach()

set(_header_check_sources "")
foreach(_header IN LISTS BLUEJAY_PUBLIC_HEADERS)
  string(MAKE_C_IDENTIFIER "${_header}" _id)
  set(_tu "${CMAKE_CURRENT_BINARY_DIR}/header_check/${_id}.cpp")
  file(GENERATE OUTPUT "${_tu}" CONTENT "#include <${_header}>\n")
  list(APPEND _header_check_sources "${_tu}")
endforeach()

add_library(bluejay_header_check OBJECT ${_header_check_sources})
target_link_libraries(bluejay_header_check PRIVATE BlueJay::Engine)
