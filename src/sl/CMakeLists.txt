set (SL_LIBS xvent spdlog jsoncpp glfw glad stb assimp imgui crossguid dl)
file (GLOB_RECURSE SL_SRC ./*.cpp)

add_library(sl STATIC ${SL_SRC})
target_include_directories(sl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
set_property(TARGET sl PROPERTY CXX_STANDARD 20)
target_link_libraries(sl PUBLIC ${SL_LIBS})

