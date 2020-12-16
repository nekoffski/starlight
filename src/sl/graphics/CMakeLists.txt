
set(SL_GRAPHICS_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_GRAPHICS_SRC ${SL_GRAPHICS_PATH}/*.cpp)

add_library(sl_graphics STATIC ${SL_GRAPHICS_SRC})
set_property(TARGET sl_graphics PROPERTY CXX_STANDARD 20)
target_link_libraries(sl_graphics PUBLIC sl_math sl_platform)

target_include_directories(sl_graphics PUBLIC ${STARLIGHT_HEADERS})
