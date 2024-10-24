add_subdirectory(src/starlight)

if(SL_ENABLE_UNIT_TESTS)
    message("-- Unit tests for starlight backend enabled")
    add_subdirectory(test)
endif()
