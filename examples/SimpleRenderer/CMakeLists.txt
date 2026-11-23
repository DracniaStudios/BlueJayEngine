# SimpleRenderer - a small software 3D renderer built on BlueJay + SDL3.
add_executable(simple_renderer main.cpp)

# BlueJay::Engine links SDL3 PRIVATE (backend types stay out of engine headers),
# so an app that opens its own window links SDL3 itself.
target_link_libraries(simple_renderer PRIVATE BlueJay::Engine SDL3::SDL3-static)

if(MSVC)
  target_compile_options(simple_renderer PRIVATE /W4 /permissive-)
else()
  target_compile_options(simple_renderer PRIVATE -Wall -Wextra)
endif()
