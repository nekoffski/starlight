include(GoogleTest)

add_executable(starlight-event-tests
    "event/EventSystem.test.cpp"
)

target_link_libraries(starlight-event-tests
    PRIVATE
        starlight
        GTest::gtest_main
)

gtest_discover_tests(starlight-event-tests)
