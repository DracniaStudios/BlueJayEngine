# CheckPublicHeaders.cmake
#
# Enforces the rule: no third-party backend header appears in a PUBLIC header.
# Backend types (raylib, SDL3, FMOD, EnTT, GameNetworkingSockets, imgui, ...)
# live under src/backend/ only, behind opaque handles.
#
# Usage: cmake -DINCLUDE_DIR=<path to include/> -P CheckPublicHeaders.cmake

if(NOT INCLUDE_DIR)
  message(FATAL_ERROR "Pass -DINCLUDE_DIR=<path to the include/ folder>")
endif()

file(GLOB_RECURSE _headers
  "${INCLUDE_DIR}/*.h" "${INCLUDE_DIR}/*.hpp" "${INCLUDE_DIR}/*.h.in")

# Matched against "/<include path>/", so every name must be followed by '/' or '.'.
set(_forbidden
  "/(raylib|raymath|rlgl|rlImGui|rlgl)[/.]"
  "/(imgui|imgui_[a-z_]*)[/.]"
  "/SDL[0-9]*[/.]"
  "/SDL_[A-Za-z_]*[/.]"
  "/(fmod|fmod_[a-z_]*)[/.]"
  "/entt[/.]"
  "/(GameNetworkingSockets|steam)[/.]"
  "/(nlohmann|glad|GLFW|vulkan)[/.]")

set(_failures 0)
foreach(_h IN LISTS _headers)
  file(STRINGS "${_h}" _lines REGEX "^[ \t]*#[ \t]*include")
  foreach(_line IN LISTS _lines)
    string(REGEX REPLACE "^[ \t]*#[ \t]*include[ \t]*[<\"]([^>\"]*)[>\"].*$" "\\1" _path "${_line}")
    foreach(_re IN LISTS _forbidden)
      if("/${_path}/" MATCHES "${_re}")
        message(SEND_ERROR "Backend header leaked into a public header:\n  ${_h}\n  ${_line}")
        math(EXPR _failures "${_failures}+1")
      endif()
    endforeach()
  endforeach()
endforeach()

list(LENGTH _headers _count)
if(_failures GREATER 0)
  message(FATAL_ERROR "${_failures} backend include(s) found in public headers.")
endif()
message(STATUS "Public headers clean (${_count} files checked).")
