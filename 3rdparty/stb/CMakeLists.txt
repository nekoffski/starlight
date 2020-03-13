cmake_minimum_required(VERSION 3.1)

project(stb)

add_library(stb STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/stb.c)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
