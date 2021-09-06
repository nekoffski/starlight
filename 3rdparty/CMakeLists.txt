add_subdirectory(glad)
add_subdirectory(stb)
add_subdirectory(libkc)

file (GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp)
add_library(imgui STATIC ${SRC})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(imgui PUBLIC glfw glad)

add_library(imguizmo STATIC ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGuizmo.cpp)
target_include_directories(imguizmo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_include_directories(imguizmo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo)
target_link_libraries(imguizmo PUBLIC imgui dl)
