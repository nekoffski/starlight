cmake_minimum_required(VERSION 3.10)
project(starlight)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug) 
# set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(SL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/engine/src)

# set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
# set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

add_definitions(-DSPDLOG_COMPILED_LIB=1)

add_definitions(-DSL_USE_GLFW=1)
add_definitions(-DSL_USE_VK=1)
add_definitions(-DSL_ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets")
add_definitions(-DSL_ASSETS_SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders")
add_definitions(-DSL_ASSETS_TEXTURES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/textures")
add_definitions(-DSL_ASSETS_MATERIALS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/materials")
add_definitions(-DSL_VK_DEBUG=1)

set(GCC_COVERAGE_COMPILE_FLAGS "--coverage -O0 -fprofile-arcs -ftest-coverage -fno-elide-constructors -fno-inline")
set(GCC_COVERAGE_LINK_FLAGS "-lgcov")

option(SL_ENABLE_UNIT_TESTS "Build unit tests" OFF)
option(SL_ENABLE_BENCHMARKS "Build benchmakrs " OFF)
option(SL_ENABLE_COVERAGE "Enable code coverage" OFF)

if(SL_ENABLE_COVERAGE)
    message("-- Triggering build with coverage support")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
    string(REPLACE "-O3" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
endif()

if(SL_ENABLE_UNIT_TESTS)
    enable_testing()
endif()

add_subdirectory(3rdparty)
add_subdirectory(engine)
# add_subdirectory(editor)
add_subdirectory(sandbox)
