
set(SL_RENDERING_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_RENDERING_SRC ${SL_RENDERING_PATH}/*.cpp)

add_library(starl_rendering STATIC ${SL_RENDERING_SRC})
target_link_libraries(starl_rendering PUBLIC starl_math starl_platform)

target_include_directories(starl_rendering PUBLIC ${STARLIGHT_HEADERS})
