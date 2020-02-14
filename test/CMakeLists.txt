set(GTEST_LINK gtest gtest_main gmock gmock_main)

add_executable(sl_test ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(sl_test starlight)
target_include_directories(sl_test PUBLIC ${STARLIGHT_HEADERS} )

file (GLOB_RECURSE SL_TEST_CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp)

add_executable(sl_core_test ${SL_TEST_CORE_SRC})    
target_link_libraries(sl_core_test PUBLIC starlight ${GTEST_LINK})
target_include_directories(sl_core_test PUBLIC ${STARLIGHT_HEADERS})
