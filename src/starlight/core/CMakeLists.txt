
set(SL_CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_CORE_SRC ${SL_CORE_PATH}/*.cpp)

add_library(starl_core STATIC ${SL_CORE_SRC})
target_link_libraries(starl_core spdlog::spdlog)

target_include_directories(starl_core PUBLIC ${STARLIGHT_HEADERS})