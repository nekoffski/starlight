#pragma once

#include <memory>

#include "sl/core/Window.h"
#include "sl/geom/ModelLoaderImpl.h"
#include "sl/gfx/Cubemap.h"
#include "sl/gfx/GraphicsContext.h"
#include "sl/gfx/Image.h"
#include "sl/gfx/RenderApi.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/ShaderCompilerImpl.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/buffer/ElementBuffer.h"
#include "sl/gfx/buffer/FrameBuffer.h"
#include "sl/gfx/buffer/RenderBuffer.h"
#include "sl/gfx/buffer/VertexArray.h"
#include "sl/gfx/buffer/VertexBuffer.h"
#include "sl/gui/GuiApi.h"

namespace sl::platform {

std::unique_ptr<sl::gfx::buffer::VertexArray::Factory> createVertexArrayFactory();
std::unique_ptr<sl::gfx::buffer::ElementBuffer::Factory> createElementBufferFactory();
std::unique_ptr<sl::gfx::buffer::VertexBuffer::Factory> createVertexBufferFactory();
std::unique_ptr<sl::gfx::buffer::FrameBuffer::Factory> createFrameBufferFactory();
std::unique_ptr<sl::gfx::Image::Factory> createImageFactory();
std::unique_ptr<sl::gfx::Texture::Factory> createTextureFactory();
std::unique_ptr<sl::gfx::Cubemap::Factory> createCubemapFactory();
std::unique_ptr<sl::gfx::Shader::Factory> createShaderFactory();
std::unique_ptr<sl::core::Window::Factory> createWindowFactory();

std::unique_ptr<sl::gui::GuiApi::Factory> createGuiApiFactory();
std::unique_ptr<sl::gfx::GraphicsContext::Factory> createGraphicsContextFactory();
std::unique_ptr<sl::gfx::ShaderCompilerImpl::Factory> createShaderCompilerImplFactory();
std::unique_ptr<geom::ModelLoaderImpl::Factory> createModelLoaderImplFactory();
std::unique_ptr<sl::gfx::RenderApi::Factory> createRenderApiFactory();
std::unique_ptr<sl::gfx::buffer::RenderBuffer::Factory> createRenderBufferFactory();

struct Platform {
    std::unique_ptr<sl::gfx::buffer::VertexArray::Factory> vertexArrayFactory = createVertexArrayFactory();
    std::unique_ptr<sl::gfx::buffer::ElementBuffer::Factory> elementBufferFactory = createElementBufferFactory();
    std::unique_ptr<sl::gfx::buffer::VertexBuffer::Factory> vertexBufferFactory = createVertexBufferFactory();
    std::unique_ptr<sl::gfx::buffer::FrameBuffer::Factory> frameBufferFactory = createFrameBufferFactory();
    std::unique_ptr<sl::gfx::Image::Factory> imageFactory = createImageFactory();
    std::unique_ptr<sl::gfx::Texture::Factory> textureFactory = createTextureFactory();
    std::unique_ptr<sl::gfx::Cubemap::Factory> cubemapFactory = createCubemapFactory();
    std::unique_ptr<sl::gfx::Shader::Factory> shaderFactory = createShaderFactory();
    std::unique_ptr<sl::core::Window::Factory> windowFactory = createWindowFactory();

    std::unique_ptr<sl::gfx::GraphicsContext::Factory> graphicsContextFactory = createGraphicsContextFactory();
    std::unique_ptr<sl::gfx::ShaderCompilerImpl::Factory> shaderCompilerFactory = createShaderCompilerImplFactory();
    std::unique_ptr<geom::ModelLoaderImpl::Factory> modelLoaderFactory = createModelLoaderImplFactory();
    std::unique_ptr<sl::gfx::RenderApi::Factory> renderApiFactory = createRenderApiFactory();
    std::unique_ptr<sl::gfx::buffer::RenderBuffer::Factory> renderBufferFactory = createRenderBufferFactory();
};

}
