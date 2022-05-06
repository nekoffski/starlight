cmake_minimum_required(VERSION 3.10)
project(nova)

set(NOVA_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/engine/src)

add_definitions(-DDEV_MODE=1)
add_definitions(-DSPDLOG_COMPILED_LIB=1)

add_subdirectory(3rdparty)
add_subdirectory(engine)
add_subdirectory(editor)
