cmake_minimum_required(VERSION 3.10)
project(starlight)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug) 

set(SL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/engine/src)

if (SL_ENABLE_SANITIZERS)
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

add_definitions(-DSL_USE_GLFW=1)
add_definitions(-DSL_USE_VK=1)
add_definitions(-DSL_VK_DEBUG=1)

set(GCC_COVERAGE_COMPILE_FLAGS "--coverage -O0 -fprofile-arcs -ftest-coverage -fno-elide-constructors -fno-inline")
set(GCC_COVERAGE_LINK_FLAGS "-lgcov")

if(SL_ENABLE_COVERAGE)
    message("-- Triggering build with coverage support")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
    string(REPLACE "-O3" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
endif()

add_subdirectory(3rdparty)

if(SL_WERR)
    set(SL_COMPILER_FLAGS -Wall -Wextra -Wpedantic -Werror)
else()
    set(SL_COMPILER_FLAGS -Wall -Wextra -Wpedantic)
endif()

find_package(VulkanLoader REQUIRED)
find_package(VulkanHeaders REQUIRED)
find_package(glfw3 REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(glm REQUIRED)
find_package(SPIRV-Cross REQUIRED)

if(SL_BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(engine)

if (SL_BUILD_EDITOR)
    add_subdirectory(editor)
endif()
