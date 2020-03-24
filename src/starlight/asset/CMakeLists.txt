
set(SL_ASSET_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_ASSET_SRC ${SL_ASSET_PATH}/*.cpp)

add_library(starl_asset STATIC ${SL_ASSET_SRC})
#target_link_libraries(starl_asset glfw glad)

target_include_directories(starl_asset PUBLIC ${STARLIGHT_HEADERS})
