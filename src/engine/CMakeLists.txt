
add_library(starlight STATIC
    # -- core
    "starlight/core/Core.cpp"
    "starlight/core/Error.cpp"
    "starlight/core/FileSystem.cpp"
    "starlight/core/Log.cpp"
    "starlight/core/Profiler.cpp"
    "starlight/core/Random.cpp"
    "starlight/core/String.cpp"
    # -- renderer
    "starlight/renderer/RendererProxy.cpp"
    "starlight/renderer/RenderingSystem.cpp"
    "starlight/renderer/backend/Renderer.cpp"
    "starlight/renderer/orchestrator/RenderOrchestrator.cpp"
    # -- platform
    "starlight/platform/Platform.cpp"
)

target_include_directories(starlight
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(spdlog CONFIG REQUIRED)
find_package(stduuid CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(stb CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

target_link_libraries(starlight
    PUBLIC
        fmt::fmt
        spdlog::spdlog
        stduuid::stduuid
        stb::stb
        sdl::sdl
)

if (APPLE)
    find_package(metal-cpp CONFIG REQUIRED)

    target_sources(starlight
        PRIVATE
            "starlight/platform/metal/MetalDevice.cpp"
            "starlight/platform/metal/Metal.cpp"
    )

    target_link_libraries(starlight
        PRIVATE
            metal-cpp::metal-cpp
    )
endif()

if (SL_ENABLE_PROFILING)
    target_compile_definitions(starlight PRIVATE SL_ENABLE_PROFILING=1)
endif()
