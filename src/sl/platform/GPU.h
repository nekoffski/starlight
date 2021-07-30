#pragma once

#include <memory>

#include "sl/gfx/Cubemap.h"
#include "sl/gfx/ElementBuffer.h"
#include "sl/gfx/FrameBuffer.h"
#include "sl/gfx/RenderApi.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/ShaderCompiler.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/VertexArray.h"
#include "sl/gfx/VertexBuffer.h"

namespace sl::platform {

struct GPU {
    explicit GPU(std::unique_ptr<gfx::VertexArray::Factory> vertexArrayFactory, std::unique_ptr<gfx::VertexBuffer::Factory> vertexBufferFactory,
        std::unique_ptr<gfx::ElementBuffer::Factory> elementBufferFactory, std::unique_ptr<gfx::FrameBuffer::Factory> frameBufferFactory,
        std::unique_ptr<gfx::RenderBuffer::Factory> renderBufferFactory, std::unique_ptr<gfx::Texture::Factory> textureFactory,
        std::unique_ptr<gfx::Cubemap::Factory> cubemapFactory, std::unique_ptr<gfx::Shader::Factory> shaderFactory, std::unique_ptr<gfx::ShaderCompiler> shaderCompiler,
        std::unique_ptr<gfx::RenderApi> renderApi)
        : vertexArrayFactory(std::move(vertexArrayFactory))
        , vertexBufferFactory(std::move(vertexBufferFactory))
        , elementBufferFactory(std::move(elementBufferFactory))
        , frameBufferFactory(std::move(frameBufferFactory))
        , renderBufferFactory(std::move(renderBufferFactory))
        , textureFactory(std::move(textureFactory))
        , shaderFactory(std::move(shaderFactory))
        , shaderCompiler(std::move(shaderCompiler))
        , renderApi(std::move(renderApi)) {
    }

    // clang-format off
    std::unique_ptr<gfx::VertexArray::Factory>   vertexArrayFactory;
    std::unique_ptr<gfx::VertexBuffer::Factory>  vertexBufferFactory;
    std::unique_ptr<gfx::ElementBuffer::Factory> elementBufferFactory;
    std::unique_ptr<gfx::FrameBuffer::Factory>   frameBufferFactory;
    std::unique_ptr<gfx::RenderBuffer::Factory>  renderBufferFactory;
    std::unique_ptr<gfx::Texture::Factory>       textureFactory;
    std::unique_ptr<gfx::Cubemap::Factory>       cubemapFactory;
    std::unique_ptr<gfx::Shader::Factory>        shaderFactory;
    std::unique_ptr<gfx::ShaderCompiler>         shaderCompiler;
    std::unique_ptr<gfx::RenderApi>              renderApi;
    // clang-format on
};

}