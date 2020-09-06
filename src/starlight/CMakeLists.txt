
add_subdirectory(platform)
add_subdirectory(core)
add_subdirectory(application)
add_subdirectory(misc)
add_subdirectory(asset)
add_subdirectory(math)
add_subdirectory(rendering)
add_subdirectory(scene)
add_subdirectory(ecs)
add_subdirectory(gui)

add_library(sl ${CMAKE_CURRENT_SOURCE_DIR}/dummy.cpp)
set_property(TARGET sl PROPERTY CXX_STANDARD 20)
target_link_libraries(sl PUBLIC sl_math sl_core sl_platform sl_asset 
                                       sl_rendering sl_application sl_ecs sl_scene sl_gui ${CMAKE_DL_LIBS})
