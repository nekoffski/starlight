
set(SL_MATH_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SL_MATH_SRC ${SL_MATH_PATH}/*.cpp)

add_library(starl_math STATIC ${SL_MATH_SRC})
target_link_libraries(starl_math PUBLIC glm::glm)

target_include_directories(starl_math PUBLIC ${STARLIGHT_HEADERS})
