set (NOVA_LIBS spdlog stb assimp dl kc fmt)
set (NOVA_TARGET nova)
file (GLOB_RECURSE NOVA_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${NOVA_TARGET} STATIC ${NOVA_SRC})
target_include_directories(${NOVA_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET ${NOVA_TARGET} PROPERTY CXX_STANDARD 20)
target_link_libraries(${NOVA_TARGET} PUBLIC ${NOVA_LIBS})
