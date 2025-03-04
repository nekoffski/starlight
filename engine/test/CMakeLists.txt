
find_package(GTest REQUIRED)

set(GTEST_LINK gtest::gtest)
set(MOCK_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})
set(SL_LIBS starlight-core starlight-renderer starlight-event starlight-window starlight-app)

function(add_test_file TEST_FILE)
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WLE)
    set(TEST_EXE_NAME starlight.${TEST_NAME})
    message("\t-> ${TEST_EXE_NAME}")
    add_executable(${TEST_EXE_NAME} ${TEST_FILE})

    target_link_libraries(${TEST_EXE_NAME} PUBLIC ${GTEST_LINK} ${SL_LIBS})
    target_include_directories(${TEST_EXE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SL_INCLUDE} ${MOCK_INCLUDE})
    add_test(NAME starlight.${TEST_NAME} COMMAND ${TEST_EXE_NAME})
endfunction()

file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Tests*.cpp)
foreach(TEST_FILE ${TEST_FILES})
    message(${TEST_FILE})
    add_test_file(${TEST_FILE} ${MODULE_NAME})
endforeach()
