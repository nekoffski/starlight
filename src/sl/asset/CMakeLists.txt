
set(SL_ASSET_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_ASSET_SRC ${SL_ASSET_PATH}/*.cpp)

add_library(sl_asset STATIC ${SL_ASSET_SRC})
set_property(TARGET sl_asset PROPERTY CXX_STANDARD 20)
target_include_directories(sl_asset PUBLIC ${STARLIGHT_HEADERS})
