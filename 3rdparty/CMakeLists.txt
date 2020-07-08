add_subdirectory(googletest)
add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(stb)
add_subdirectory(glm)
add_subdirectory(jsoncpp)
add_subdirectory(assimp)

file (GLOB IMGUI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui/*.cpp)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(imgui PUBLIC glfw glad)

add_library(easyloggingpp STATIC ${CMAKE_CURRENT_SOURCE_DIR}/easyloggingpp/src/easylogging++.cc)
target_include_directories(easyloggingpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/easyloggingpp/src)
