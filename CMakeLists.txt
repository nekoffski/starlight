cmake_minimum_required(VERSION 3.10)
project(starlight)

set(CMAKE_CXX_STANDARD 20)
set(STARLIGHT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(THIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

set(GCC_COVERAGE_COMPILE_FLAGS "--coverage -O0 -fprofile-arcs -ftest-coverage -fno-elide-constructors -fno-inline")
set(GCC_COVERAGE_LINK_FLAGS    "-lgcov")

add_subdirectory(3rdparty)

find_package(Threads)
enable_testing()

if (COV_BUILD)
	message("-- Triggering build with coverage support")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
	string(REPLACE "-O3" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
else()
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2")
endif()

add_definitions(-DSHADERS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/shaders")
add_definitions(-DTEXTURES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/textures")
add_definitions(-DCUBEMAPS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/cubemaps")
add_definitions(-DMODELS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/models")
add_definitions(-DRES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/")

add_subdirectory(src)
add_subdirectory(test)

