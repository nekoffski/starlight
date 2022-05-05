#pragma once

#include <memory>

#include "sl/gpu/Cubemap.h"
#include "sl/gpu/ElementBuffer.h"
#include "sl/gpu/FrameBuffer.h"
#include "sl/gpu/RenderApi.h"
#include "sl/gpu/Shader.h"
#include "sl/gpu/ShaderCompiler.h"
#include "sl/gpu/Texture.h"
#include "sl/gpu/VertexArray.h"
#include "sl/gpu/VertexBuffer.h"

namespace sl::platform {

struct GPU {
    explicit GPU(
        std::unique_ptr<gpu::VertexArray::Factory> vertexArrayFactory,
        std::unique_ptr<gpu::VertexBuffer::Factory> vertexBufferFactory,
        std::unique_ptr<gpu::ElementBuffer::Factory> elementBufferFactory,
        std::unique_ptr<gpu::FrameBuffer::Factory> frameBufferFactory,
        std::unique_ptr<gpu::RenderBuffer::Factory> renderBufferFactory,
        std::unique_ptr<gpu::Texture::Factory> textureFactory,
        std::unique_ptr<gpu::Cubemap::Factory> cubemapFactory,
        std::unique_ptr<gpu::Shader::Factory> shaderFactory,
        std::unique_ptr<gpu::ShaderCompiler> shaderCompiler,
        std::unique_ptr<gpu::RenderApi> renderApi
    )
        : vertexArrayFactory(std::move(vertexArrayFactory))
        , vertexBufferFactory(std::move(vertexBufferFactory))
        , elementBufferFactory(std::move(elementBufferFactory))
        , frameBufferFactory(std::move(frameBufferFactory))
        , renderBufferFactory(std::move(renderBufferFactory))
        , textureFactory(std::move(textureFactory))
        , cubemapFactory(std::move(cubemapFactory))
        , shaderFactory(std::move(shaderFactory))
        , shaderCompiler(std::move(shaderCompiler))
        , renderApi(std::move(renderApi)) {}

    // clang-format off
    std::unique_ptr<gpu::VertexArray::Factory>   vertexArrayFactory;
    std::unique_ptr<gpu::VertexBuffer::Factory>  vertexBufferFactory;
    std::unique_ptr<gpu::ElementBuffer::Factory> elementBufferFactory;
    std::unique_ptr<gpu::FrameBuffer::Factory>   frameBufferFactory;
    std::unique_ptr<gpu::RenderBuffer::Factory>  renderBufferFactory;
    std::unique_ptr<gpu::Texture::Factory>       textureFactory;
    std::unique_ptr<gpu::Cubemap::Factory>       cubemapFactory;
    std::unique_ptr<gpu::Shader::Factory>        shaderFactory;
    std::unique_ptr<gpu::ShaderCompiler>         shaderCompiler;
    std::unique_ptr<gpu::RenderApi>              renderApi;
    // clang-format on
};

}  // namespace sl::platform