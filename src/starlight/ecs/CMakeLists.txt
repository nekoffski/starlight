
set(SL_ECS_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_ECS_SRC ${SL_ECS_PATH}/*.cpp)

add_library(starl_ecs STATIC ${SL_ECS_SRC})
target_link_libraries(starl_ecs PUBLIC starl_math starl_core starl_scene)

target_include_directories(starl_ecs PUBLIC ${STARLIGHT_HEADERS})
