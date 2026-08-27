cmake_minimum_required(VERSION 3.22)
project(starlight VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(STARLIGHT_ENABLE_PROFILING OFF)

find_package(spdlog REQUIRED)
find_package(stduuid REQUIRED)
find_package(GTest REQUIRED)
find_package(stb REQUIRED)

include(CTest)

add_subdirectory(src)
