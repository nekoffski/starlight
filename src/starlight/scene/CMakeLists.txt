
set(SL_SCENE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_SCENE_SRC ${SL_SCENE_PATH}/*.cpp)

add_library(starl_scene STATIC ${SL_SCENE_SRC})
target_link_libraries(starl_scene PUBLIC starl_math starl_core starl_gui starl_rendering)

target_include_directories(starl_scene PUBLIC ${STARLIGHT_HEADERS})
