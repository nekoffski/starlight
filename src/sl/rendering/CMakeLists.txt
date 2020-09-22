
set(SL_RENDERING_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_RENDERING_SRC ${SL_RENDERING_PATH}/*.cpp)

add_library(sl_rendering STATIC ${SL_RENDERING_SRC})
set_property(TARGET sl_rendering PROPERTY CXX_STANDARD 20)
target_link_libraries(sl_rendering PUBLIC sl_math sl_platform)

target_include_directories(sl_rendering PUBLIC ${STARLIGHT_HEADERS})
