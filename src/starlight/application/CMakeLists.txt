
set(SL_APP_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_APP_SRC ${SL_APP_PATH}/*.cpp)

add_library(starl_application STATIC ${SL_APP_SRC})
set_property(TARGET starl_application PROPERTY CXX_STANDARD 20)
target_include_directories(starl_application PUBLIC ${STARLIGHT_HEADERS})
target_link_libraries(starl_application PUBLIC starl_platform starl_framework starl_rendering starl_gui)