
set(SL_FRAMEWORK_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_FRAMEWORK_SRC ${SL_FRAMEWORK_PATH}/*.cpp)

add_library(starl_framework STATIC ${SL_FRAMEWORK_SRC})
target_link_libraries(starl_framework PUBLIC starl_math starl_core)

target_include_directories(starl_framework PUBLIC ${STARLIGHT_HEADERS})
