
add_subdirectory(platform)
add_subdirectory(core)
add_subdirectory(application)

add_library(starlight ${CMAKE_CURRENT_SOURCE_DIR}/dummy.cpp)
target_link_libraries(starlight starl_core)
