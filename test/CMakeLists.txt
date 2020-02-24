set(GTEST_LINK gtest gtest_main gmock gmock_main)

add_executable(sl_test ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(sl_test starlight)
target_include_directories(sl_test PUBLIC ${STARLIGHT_HEADERS} )

file (GLOB_RECURSE SL_TEST_CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp)
file (GLOB_RECURSE SL_TEST_PLATFORM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/platform/*.cpp)

add_executable(sl_core_test ${SL_TEST_CORE_SRC})    
target_link_libraries(sl_core_test PUBLIC starlight ${GTEST_LINK})
target_include_directories(sl_core_test PUBLIC ${STARLIGHT_HEADERS})

add_executable(sl_platform_test ${SL_TEST_PLATFORM_SRC})    
target_link_libraries(sl_platform_test PUBLIC starlight ${GTEST_LINK})
target_include_directories(sl_platform_test PUBLIC ${STARLIGHT_HEADERS})
