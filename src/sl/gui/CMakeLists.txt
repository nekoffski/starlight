
set(SL_GUI_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_GUI_SRC ${SL_GUI_PATH}/*.cpp)

add_library(sl_gui STATIC ${SL_GUI_SRC})
set_property(TARGET sl_gui PROPERTY CXX_STANDARD 20)
target_link_libraries(sl_gui PUBLIC sl_platform stdc++fs)

target_include_directories(sl_gui PUBLIC ${STARLIGHT_HEADERS})
