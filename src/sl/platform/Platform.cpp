#include "Platform.h"

#include <memory>

#include "core/GlfwInput.h"
#include "core/GlfwWindow.h"
#include "core/StbImage.hpp"
#include "gfx/OpenGlCubemap.h"
#include "gfx/OpenGlElementBuffer.h"
#include "gfx/OpenGlFrameBuffer.h"
#include "gfx/OpenGlGraphicsContext.h"
#include "gfx/OpenGlRenderApi.h"
#include "gfx/OpenGlRenderBuffer.h"
#include "gfx/OpenGlShader.h"
#include "gfx/OpenGlShaderCompilerImpl.h"
#include "gfx/OpenGlTexture.h"
#include "gfx/OpenGlVertexArray.h"
#include "gfx/OpenGlVertexBuffer.h"
#include "gui/ImGuiApi.h"
#include "model/AssimpModelLoaderImpl.h"
#include "sl/core/Logger.h"
#include "sl/platform/PlatformDetector.h"

namespace sl::platform {

std::unique_ptr<sl::gfx::buffer::RenderBuffer::Factory> createRenderBufferFactory() {
    return std::make_unique<gfx::OpenGlRenderBufferFactory>();
}

std::unique_ptr<sl::gfx::RenderApi::Factory> createRenderApiFactory() {
    return std::make_unique<gfx::OpenGlRenderApiFactory>();
}

std::unique_ptr<geom::ModelLoaderImpl::Factory> createModelLoaderImplFactory() {
    return std::make_unique<model::AssimpModelLoaderImplFactory>();
}

std::unique_ptr<sl::gfx::ShaderCompilerImpl::Factory> createShaderCompilerImplFactory() {
    return std::make_unique<gfx::OpenGlShaderCompilerImplFactory>();
}

std::unique_ptr<sl::gfx::GraphicsContext::Factory> createGraphicsContextFactory() {
    return std::make_unique<gfx::OpenGlGraphicsContextFactory>();
}

std::unique_ptr<sl::core::Window::Factory> createWindowFactory() {
    return std::make_unique<core::GlfwWindowFactory>();
}

std::unique_ptr<sl::core::Input::Factory> createInputFactory() {
    return std::make_unique<core::GlfwInputFactory>();
}

std::unique_ptr<sl::gui::GuiApi::Factory> createGuiApiFactory() {
    return std::make_unique<sl::platform::gui::ImGuiApiFactory>();
}

std::unique_ptr<sl::gfx::Texture::Factory> createTextureFactory() {
    return std::make_unique<gfx::OpenGlTextureFactory>();
}

std::unique_ptr<sl::gfx::Cubemap::Factory> createCubemapFactory() {
    return std::make_unique<gfx::OpenGlCubemapFactory>();
}

std::unique_ptr<sl::gfx::buffer::VertexArray::Factory> createVertexArrayFactory() {
    return std::make_unique<gfx::OpenGlVertexArrayFactory>();
}

std::unique_ptr<sl::gfx::buffer::ElementBuffer::Factory> createElementBufferFactory() {
    return std::make_unique<gfx::OpenGlElementBufferFactory>();
}

std::unique_ptr<sl::gfx::buffer::VertexBuffer::Factory> createVertexBufferFactory() {
    return std::make_unique<gfx::OpenGlVertexBufferFactory>();
}

std::unique_ptr<sl::gfx::buffer::FrameBuffer::Factory> createFrameBufferFactory() {
    return std::make_unique<gfx::OpenGlFrameBufferFactory>();
}

std::unique_ptr<sl::gfx::Image::Factory> createImageFactory() {
    return std::make_unique<gfx::StbImageFactory>();
}

std::unique_ptr<sl::gfx::Shader::Factory> createShaderFactory() {
    return std::make_unique<gfx::OpenGlShaderFactory>();
}
}
