include(GoogleTest)

function(add_starlight_test target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} PRIVATE starlight GTest::gtest_main)
    gtest_discover_tests(${target})
endfunction()

add_starlight_test(starlight-event-tests
    "event/EventSystem.test.cpp"
)

add_starlight_test(starlight-runtime-tests
    "runtime/ThreadSafeQueue.test.cpp"
)

add_starlight_test(starlight-core-tests
    "core/FlatMap.test.cpp"
)
