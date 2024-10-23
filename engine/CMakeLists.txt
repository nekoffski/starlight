add_subdirectory(src/starlight)

if(ENABLE_TESTS)
    message("-- Unit tests for starlight backend enabled")
    # add_subdirectory(test)
endif()
