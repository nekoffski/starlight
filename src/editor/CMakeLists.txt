
add_executable(starlight-editor ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
set_property(TARGET starlight-editor PROPERTY CXX_STANDARD 20)
target_link_libraries(starlight-editor PUBLIC sl)
target_include_directories(starlight-editor PUBLIC ${STARLIGHT_HEADERS})
