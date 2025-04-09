add_subdirectory(src/starlight)

if(SL_BUILD_TESTS)
    message("-- Unit tests for starlight backend enabled")
    add_subdirectory(test)
endif()

if (SL_BUILD_BENCHMARKS)
    message("-- Benchmarks for starlight backend enabled")
    add_subdirectory(bench)
endif()

if (SL_BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif()
