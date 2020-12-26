
set(SL_APP_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_APP_SRC ${SL_APP_PATH}/*.cpp)

add_library(sl_application STATIC ${SL_APP_SRC})
set_property(TARGET sl_application PROPERTY CXX_STANDARD 20)
target_include_directories(sl_application PUBLIC ${STARLIGHT_HEADERS})
target_link_libraries(sl_application PUBLIC sl_platform sl_graphics sl_gui sl_scene sl_async)
