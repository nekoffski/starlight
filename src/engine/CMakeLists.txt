
add_library(starlight STATIC
    # -- core
    "starlight/core/Core.cpp"
    "starlight/core/Error.cpp"
    "starlight/core/FileSystem.cpp"
    "starlight/core/Log.cpp"
    "starlight/core/Profiler.cpp"
    "starlight/core/Random.cpp"
    "starlight/core/String.cpp"
    "starlight/core/Id.cpp"
    # -- event
    "starlight/event/EventBus.cpp"
    "starlight/event/EventSystem.cpp"
    "starlight/event/details/EventBroker.cpp"
    # -- renderer
    "starlight/renderer/RendererProxy.cpp"
    "starlight/renderer/RenderingSystem.cpp"
    "starlight/renderer/backend/Renderer.cpp"
    "starlight/renderer/executor/RenderExecutor.cpp"
    "starlight/renderer/executor/RendererCommandDispatcher.cpp"
    # -- platform
    "starlight/platform/Platform.cpp"
    "starlight/platform/sdl/SDLWindow.cpp"
    # -- runtime
    "starlight/runtime/Thread.cpp"
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
find_package(glm CONFIG REQUIRED)

target_link_libraries(starlight
    PUBLIC
        fmt::fmt
        spdlog::spdlog
        stduuid::stduuid
        stb::stb
        sdl::sdl
        glm::glm
)

if (APPLE)
    find_package(metal-cpp CONFIG REQUIRED)

    target_sources(starlight
        PRIVATE
            "starlight/platform/metal/Metal.cpp"
            "starlight/platform/metal/MetalDevice.cpp"
            "starlight/platform/metal/MetalResourcePool.cpp"
            "starlight/platform/metal/MetalRenderFrameLatch.cpp"
            "starlight/platform/metal/MetalRenderFrameRecorder.cpp"
    )

    target_compile_definitions(starlight PUBLIC
        SL_PLATFORM_DARWIN=1
        SL_USE_METAL=1
    )

    target_link_libraries(starlight
        PUBLIC
            metal-cpp::metal-cpp
    )
endif()

if (SL_ENABLE_PROFILING)
    target_compile_definitions(starlight PRIVATE SL_ENABLE_PROFILING=1)
endif()

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
