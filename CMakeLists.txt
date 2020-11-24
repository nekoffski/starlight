cmake_minimum_required(VERSION 3.10)
project(starlight)

set(CMAKE_CXX_STANDARD 20)
set(STARLIGHT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(THIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
set(FL_HOME ${THIRDPARTY}/fuzzylite)
set(FL_LIBRARY_DIR ${FL_HOME}/bin)

set(FL_LIBRARY_NAME  fuzzylite)
find_library (FuzzyLiteLibrary ${FL_LIBRARY_NAME} HINTS ${FL_LIBRARY_DIR})

add_definitions(-DSHADERS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/shaders")
add_definitions(-DTEXTURES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/textures")
add_definitions(-DCUBEMAPS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/cubemaps")
add_definitions(-DMODELS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/models")
add_definitions(-DRES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/")

include_directories(${THIRDPARTY}/fuzzylite/fuzzylite)

enable_testing()

add_subdirectory(3rdparty)
add_subdirectory(src)
add_subdirectory(test)

