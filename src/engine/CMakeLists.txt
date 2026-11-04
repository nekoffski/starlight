
add_library(starlight STATIC
    # -- asset
    "starlight/asset/AssetSystem.cpp"
    "starlight/asset/Shader.cpp"
    "starlight/asset/loaders/ShaderLoader.cpp"
    "starlight/asset/loaders/ShaderReflection.cpp"
    "starlight/asset/Mesh.cpp"
    "starlight/asset/Texture.cpp"
    # -- core
    "starlight/core/Error.cpp"
    # -- event
    "starlight/event/EventBus.cpp"
    "starlight/event/EventSystem.cpp"
    "starlight/event/details/EventBroker.cpp"
    # -- renderer
    "starlight/renderer/core/Shader.cpp"
    "starlight/renderer/core/Vertex.cpp"
    "starlight/renderer/core/ShaderDataLayout.cpp"
    "starlight/renderer/shading/Material.cpp"
    "starlight/renderer/RendererProxy.cpp"
    "starlight/renderer/RenderingSystem.cpp"
    "starlight/renderer/frontend/Renderer.cpp"
    "starlight/renderer/frontend/RendererResourcePool.cpp"
    "starlight/renderer/frontend/RenderGraph.cpp"
    "starlight/renderer/frontend/RenderRequestBuilder.cpp"
    "starlight/renderer/executor/RenderExecutor.cpp"
    "starlight/renderer/executor/RendererCommandDispatcher.cpp"
    # -- platform
    "starlight/platform/Platform.cpp"
    "starlight/platform/sdl/SDLWindow.cpp"
    # -- runtime
    "starlight/runtime/EventLoop.cpp"
)

target_include_directories(starlight
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(stb CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)


target_link_libraries(starlight
    PUBLIC
        kcore::kcore
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
            "starlight/platform/metal/MetalRenderPass.cpp"
            "starlight/platform/metal/MetalShader.cpp"
            "starlight/platform/metal/MetalGraphicsPipeline.cpp"
            "starlight/platform/metal/MetalBuffer.cpp"
    )

    target_compile_definitions(starlight PUBLIC
        SL_PLATFORM_DARWIN=1
        SL_USE_METAL=1
    )

    target_link_libraries(starlight
        PUBLIC
            metal-cpp::metal-cpp
    )

    target_compile_options(starlight PRIVATE -fblocks)
endif()

if (LINUX)

    target_sources(starlight
        PRIVATE
            "starlight/platform/vulkan/Vulkan.cpp"
            # "starlight/platform/vulkan/VulkanDevice.cpp"
            # "starlight/platform/vulkan/VulkanResourcePool.cpp"
            # "starlight/platform/vulkan/VulkanRenderFrameLatch.cpp"
            # "starlight/platform/vulkan/VulkanRenderFrameRecorder.cpp"
            # "starlight/platform/vulkan/VulkanRenderPass.cpp"
            # "starlight/platform/vulkan/VulkanShader.cpp"
            # "starlight/platform/vulkan/VulkanGraphicsPipeline.cpp"
            # "starlight/platform/vulkan/VulkanBuffer.cpp"
    )

    target_compile_definitions(starlight PUBLIC
        SL_PLATFORM_LINUX=1
        SL_USE_VULKAN=1
    )

    target_compile_options(starlight PRIVATE -pthread)

endif()

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
