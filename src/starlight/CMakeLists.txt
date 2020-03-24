
add_subdirectory(platform)
add_subdirectory(core)
add_subdirectory(application)
add_subdirectory(misc)
add_subdirectory(asset)
add_subdirectory(framework)
add_subdirectory(math)
add_subdirectory(rendering)

add_library(starlight ${CMAKE_CURRENT_SOURCE_DIR}/dummy.cpp)
target_link_libraries(starlight PUBLIC starl_math starl_core starl_platform starl_asset starl_framework starl_rendering)
