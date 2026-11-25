# SimpleRenderer - a small software 3D renderer built on BlueJay.
add_executable(simple_renderer main.cpp SpinningShapes.cpp)

# The engine owns the window and input, so the example only needs BlueJay.
target_link_libraries(simple_renderer PRIVATE BlueJay::Engine)

if(MSVC)
  target_compile_options(simple_renderer PRIVATE /W4 /permissive-)
else()
  target_compile_options(simple_renderer PRIVATE -Wall -Wextra)
endif()
