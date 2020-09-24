
set(SL_MATH_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_MATH_SRC ${SL_MATH_PATH}/*.cpp)

add_library(sl_math STATIC ${SL_MATH_SRC})
set_property(TARGET sl_math PROPERTY CXX_STANDARD 20)

target_include_directories(sl_math PUBLIC ${STARLIGHT_HEADERS})
