project(starlight)

cmake_minimum_required(VERSION 3.10)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(STARLIGHT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(3rdparty)
add_subdirectory(src/starlight)
add_subdirectory(test)
