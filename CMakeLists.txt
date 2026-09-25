cmake_minimum_required(VERSION 3.25)
project(starlight VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SL_ENABLE_PROFILING OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(SL_MIN_CXX_COMPILER_VERSION 14)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(SL_MIN_CXX_COMPILER_VERSION 18)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(SL_MIN_CXX_COMPILER_VERSION 17)
else()
    message(FATAL_ERROR
        "Unsupported C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}. "
        "Use GCC 14+, Clang 18+, or Apple Clang 17+ (Xcode 16.3+).")
endif()

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS SL_MIN_CXX_COMPILER_VERSION)
    message(FATAL_ERROR
        "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} is too old. "
        "This project requires GCC 14+, Clang 18+, or Apple Clang 17+ (Xcode 16.3+).")
endif()

include(CTest)
add_subdirectory(src)
