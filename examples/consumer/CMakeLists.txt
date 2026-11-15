cmake_minimum_required(VERSION 3.25)
project(BlueJayConsumerExample LANGUAGES CXX)

# This example imports BlueJay exactly the way Project Veil does.
# Default: LOCAL mode against this checkout (two folders up).
# Try the others:
#   -DBLUEJAY_SOURCE=PACKAGE -DCMAKE_PREFIX_PATH=<install prefix>
#   -DBLUEJAY_SOURCE=GIT -DBLUEJAY_GIT_URL=<url> -DBLUEJAY_GIT_TAG=<tag>
set(BLUEJAY_LOCAL_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE PATH "")
include("${CMAKE_CURRENT_LIST_DIR}/../../cmake/ImportBlueJay.cmake")

add_executable(consumer main.cpp)
target_link_libraries(consumer PRIVATE BlueJay::Engine)
