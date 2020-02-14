project(starlight)

cmake_minimum_required(VERSION 3.10)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(STARLIGHT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SPDLOG_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/spdlog/include)
set(GTEST_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/googletest/include ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/googlemock/include)

include_directories(SPDLOG_HEADERS GTEST_HEADERS)

add_subdirectory(3rdparty)
add_subdirectory(src/starlight)
add_subdirectory(test)
