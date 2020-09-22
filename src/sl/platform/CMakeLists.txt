
set(SL_PLATFORM_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_PLATFORM_SRC ${SL_PLATFORM_PATH}/*.cpp)

add_library(sl_platform STATIC ${SL_PLATFORM_SRC})
set_property(TARGET sl_platform PROPERTY CXX_STANDARD 20)
target_link_libraries(sl_platform PUBLIC glfw glad stb sl_math sl_core assimp imgui)

target_include_directories(sl_platform PUBLIC ${STARLIGHT_HEADERS})
