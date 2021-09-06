set (LOG_LIBS spdlog glad stb assimp imgui dl imguizmo kc fmt)
file (GLOB_RECURSE LOG_SRC ./*.cpp)

add_library(sl STATIC ${LOG_SRC})
target_include_directories(sl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
set_property(TARGET sl PROPERTY CXX_STANDARD 20)
target_link_libraries(sl PUBLIC ${LOG_LIBS})

