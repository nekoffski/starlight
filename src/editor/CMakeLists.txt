
file(GLOB_RECURSE src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(starlight-editor ${src})
set_property(TARGET starlight-editor PROPERTY CXX_STANDARD 20)
target_link_libraries(starlight-editor PUBLIC sl stdc++fs)
target_include_directories(starlight-editor PUBLIC ${STARLIGHT_HEADERS})
