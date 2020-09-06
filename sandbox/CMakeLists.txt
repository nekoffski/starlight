
add_executable(starlight ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
set_property(TARGET starlight PROPERTY CXX_STANDARD 20)
target_link_libraries(starlight PUBLIC sl)
target_include_directories(starlight PUBLIC ${STARLIGHT_HEADERS})
