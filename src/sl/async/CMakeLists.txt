
set(SL_ASYNC_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_ASYNC_SRC ${SL_ASYNC_PATH}/*.cpp)

add_library(sl_async STATIC ${SL_ASYNC_SRC})
set_property(TARGET sl_async PROPERTY CXX_STANDARD 20)

target_include_directories(sl_async PUBLIC ${STARLIGHT_HEADERS})
