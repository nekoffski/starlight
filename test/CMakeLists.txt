set(GTEST_LINK gtest gtest_main gmock gmock_main)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fconcepts")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fconcepts")

add_executable(starlight_main ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
set_property(TARGET starlight_main PROPERTY CXX_STANDARD 20)
target_link_libraries(starlight_main PUBLIC starlight)

target_include_directories(starlight_main PUBLIC ${STARLIGHT_HEADERS})

file (GLOB_RECURSE SL_TEST_CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp)
file (GLOB_RECURSE SL_TEST_PLATFORM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/platform/*.cpp)
file (GLOB_RECURSE SL_TEST_MISC_SRC ${CMAKE_CURRENT_SOURCE_DIR}/misc/*.cpp)

add_executable(sl_core_test ${SL_TEST_CORE_SRC})    
target_link_libraries(sl_core_test PUBLIC starlight ${GTEST_LINK})
target_include_directories(sl_core_test PUBLIC ${STARLIGHT_HEADERS})

add_executable(sl_platform_test ${SL_TEST_PLATFORM_SRC})    
target_link_libraries(sl_platform_test PUBLIC starlight ${GTEST_LINK})
target_include_directories(sl_platform_test PUBLIC ${STARLIGHT_HEADERS})

add_executable(sl_misc_test ${SL_TEST_MISC_SRC})    
target_link_libraries(sl_misc_test PUBLIC starlight ${GTEST_LINK})
target_include_directories(sl_misc_test PUBLIC ${STARLIGHT_HEADERS})
