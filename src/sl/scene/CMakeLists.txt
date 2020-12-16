
set(SL_SCENE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_SCENE_SRC ${SL_SCENE_PATH}/*.cpp)

add_library(sl_scene STATIC ${SL_SCENE_SRC})
set_property(TARGET sl_scene PROPERTY CXX_STANDARD 20)
target_link_libraries(sl_scene PUBLIC sl_math sl_core sl_ecs sl_gui sl_graphics sl_ai sl_async)

target_include_directories(sl_scene PUBLIC ${STARLIGHT_HEADERS})
