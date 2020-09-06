set(GTEST_LINK gtest gtest_main gmock gmock_main)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fconcepts")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fconcepts")

include(GoogleTest)

function(sl_add_test bin files)
    add_executable(${bin} ${files})
    target_link_libraries(${bin} PUBLIC sl ${GTEST_LINK})
    target_include_directories(${bin} PUBLIC ${STARLIGHT_HEADERS})    
    gtest_add_tests(TARGET ${bin})
endfunction()

file (GLOB_RECURSE SL_TEST_CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp)
file (GLOB_RECURSE SL_TEST_PLATFORM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/platform/*.cpp)
file (GLOB_RECURSE SL_TEST_MISC_SRC ${CMAKE_CURRENT_SOURCE_DIR}/misc/*.cpp)


sl_add_test(core_tests "${SL_TEST_CORE_SRC}")
# sl_add_test(platform_tests ${SL_TEST_PLATFORM_SRC})
sl_add_test(misc_tests "${SL_TEST_MISC_SRC}")


