
set(SL_CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_CORE_SRC ${SL_CORE_PATH}/*.cpp)

add_library(sl_core STATIC ${SL_CORE_SRC})
set_property(TARGET sl_core PROPERTY CXX_STANDARD 20)
target_link_libraries(sl_core PUBLIC spdlog)

target_include_directories(sl_core PUBLIC ${STARLIGHT_HEADERS})
