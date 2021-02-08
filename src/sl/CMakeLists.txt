
add_subdirectory(platform)
add_subdirectory(core)
add_subdirectory(application)
add_subdirectory(asset)
add_subdirectory(math)
add_subdirectory(graphics)
add_subdirectory(scene)
add_subdirectory(ecs)
add_subdirectory(gui)
add_subdirectory(async)


add_compile_options(-lstdc++fs)
add_library(sl ${CMAKE_CURRENT_SOURCE_DIR}/dummy.cpp)
set_property(TARGET sl PROPERTY CXX_STANDARD 20)
target_link_libraries(sl PUBLIC sl_math sl_core sl_platform sl_asset sl_async
                                       sl_graphics sl_application sl_ecs sl_scene sl_gui ${CMAKE_DL_LIBS} stdc++fs)


