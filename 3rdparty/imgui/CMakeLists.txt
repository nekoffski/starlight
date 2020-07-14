file (GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp)

add_library(imgui STATIC ${SRC})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(imgui PUBLIC glfw glad)