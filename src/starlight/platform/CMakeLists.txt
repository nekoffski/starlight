
set(SL_PLATFORM_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_PLATFORM_SRC ${SL_PLATFORM_PATH}/*.cpp)

add_library(starl_platform STATIC ${SL_PLATFORM_SRC})
target_link_libraries(starl_platform PUBLIC glfw glad stb starl_math)

target_include_directories(starl_platform PUBLIC ${STARLIGHT_HEADERS})
