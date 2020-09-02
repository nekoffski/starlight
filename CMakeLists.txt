cmake_minimum_required(VERSION 3.10)
project(starlight)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(STARLIGHT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_definitions(-DSHADERS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/shaders")
add_definitions(-DTEXTURES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/textures")
add_definitions(-DCUBEMAPS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/cubemaps")
add_definitions(-DMODELS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/models")

add_subdirectory(3rdparty)
add_subdirectory(src/starlight)
add_subdirectory(test)
