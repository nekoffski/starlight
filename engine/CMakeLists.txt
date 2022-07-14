add_compile_definitions(NV_VK_DEBUG)

add_subdirectory(src/nova)

if(ENABLE_TESTS)
    message("-- Unit tests for nova backend enabled")
    add_subdirectory(test)
endif()
