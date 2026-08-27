
add_library(starlight STATIC
    # -- core
    "starlight/core/Core.cpp"
    "starlight/core/Error.cpp"
    "starlight/core/FileSystem.cpp"
    "starlight/core/Log.cpp"
    "starlight/core/OS.cpp"
    "starlight/core/Profiler.cpp"
    "starlight/core/Random.cpp"
    "starlight/core/String.cpp"
)

target_include_directories(starlight
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(starlight
    PUBLIC
        fmt::fmt
        spdlog::spdlog
        stduuid::stduuid
        stb::stb
)

if (STARLIGHT_ENABLE_PROFILING)
    target_compile_definitions(starlight PRIVATE STARLIGHT_ENABLE_PROFILING=1)
endif()
