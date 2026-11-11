find_package(GTest CONFIG REQUIRED)
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
    "runtime/EventLoop.test.cpp"
)

add_starlight_test(starlight-core-tests
    "core/FlatMap.test.cpp"
    "core/Error.test.cpp"
)

add_starlight_test(starlight-renderer-tests
    "renderer/Textures.test.cpp"
    "renderer/Materials.test.cpp"
)

add_starlight_test(starlight-asset-tests
    "asset/ShaderReflection.test.cpp"
    "asset/AssetSystem.test.cpp"
)

if (APPLE)
    add_starlight_test(starlight-metal-texture-tests
        "renderer/MetalTextures.test.cpp"
    )
endif()
