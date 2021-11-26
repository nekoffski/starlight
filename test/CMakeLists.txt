set(GTEST_LINK gtest gtest_main gmock gmock_main)
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fconcepts")
# set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fconcepts")

add_executable(manual_test manual_test.cpp)
target_link_libraries(manual_test PUBLIC sl)
target_include_directories(manual_test PUBLIC ${STARLIGHT_HEADERS})

include(GoogleTest)

function(sl_add_test module)
	file (GLOB_RECURSE src ${CMAKE_CURRENT_SOURCE_DIR}/${module}/*.cpp)
	add_executable(${module}_tests ${src})
	target_link_libraries(${module}_tests PUBLIC sl ${GTEST_LINK})
	target_include_directories(${module}_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
	set_property(TARGET ${module}_tests PROPERTY CXX_STANDARD 20)
	gtest_add_tests(TARGET ${module}_tests)
endfunction()


sl_add_test(core)
sl_add_test(math)
sl_add_test(app)
sl_add_test(asset)
sl_add_test(ecs)
sl_add_test(platform)
sl_add_test(async)
