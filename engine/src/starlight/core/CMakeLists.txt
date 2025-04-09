set(SL_CORE_LIBS kstd::kstd stb dl glm::glm)
set(SL_CORE_TARGET starlight-core)
file(GLOB_RECURSE SL_CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${SL_CORE_TARGET} STATIC ${SL_CORE_SRC})
target_include_directories(${SL_CORE_TARGET} PRIVATE ${SL_INCLUDE})
target_link_libraries(${SL_CORE_TARGET} PUBLIC ${SL_CORE_LIBS})
target_compile_options(${SL_CORE_TARGET} PRIVATE ${SL_COMPILER_FLAGS})
