set(SL_PHYSX_LIBS dl kstd starlight-core)
set(SL_PHYSX_TARGET starlight-physx)
file(GLOB_RECURSE SL_PHYSX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${SL_PHYSX_TARGET} STATIC ${SL_PHYSX_SRC})
target_include_directories(${SL_PHYSX_TARGET} PRIVATE ${SL_INCLUDE})
target_link_libraries(${SL_PHYSX_TARGET} PUBLIC ${SL_PHYSX_LIBS})
target_compile_options(${SL_PHYSX_TARGET} PRIVATE ${SL_COMPILER_FLAGS})
