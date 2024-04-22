set(SL_LIBS spdlog dl kc fmt backward starlight-core starlight-renderer)
set(SL_TARGET starlight-ui)
file(GLOB_RECURSE SL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${SL_TARGET} STATIC ${SL_SRC})
target_include_directories(${SL_TARGET} PRIVATE ${SL_INCLUDE})
set_property(TARGET ${SL_TARGET} PROPERTY CXX_STANDARD 20)
target_link_libraries(${SL_TARGET} PUBLIC ${SL_LIBS})
