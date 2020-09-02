
set(SL_ECS_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_ECS_SRC ${SL_ECS_PATH}/*.cpp)

add_library(sl_ecs STATIC ${SL_ECS_SRC})
set_property(TARGET sl_ecs PROPERTY CXX_STANDARD 20)
target_link_libraries(sl_ecs PUBLIC sl_math sl_core sl_scene crossguid)

target_include_directories(sl_ecs PUBLIC ${STARLIGHT_HEADERS})
