set(SL_MATH_LIBS kstd::kstd glm::glm starlight-core)
set(SL_MATH_TARGET starlight-math)
file(GLOB_RECURSE SL_MATH_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${SL_MATH_TARGET} STATIC ${SL_MATH_SRC})
target_include_directories(${SL_MATH_TARGET} PRIVATE ${SL_INCLUDE})
target_link_libraries(${SL_MATH_TARGET} PUBLIC ${SL_MATH_LIBS})
target_compile_options(${SL_MATH_TARGET} PRIVATE ${SL_COMPILER_FLAGS})
