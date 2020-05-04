
set(SL_GUI_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_GUI_SRC ${SL_GUI_PATH}/*.cpp)

add_library(starl_gui STATIC ${SL_GUI_SRC})
target_link_libraries(starl_gui PUBLIC starl_platform)

target_include_directories(starl_gui PUBLIC ${STARLIGHT_HEADERS})
