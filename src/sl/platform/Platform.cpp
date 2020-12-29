#include "Platform.h"

#include <memory>

#include "gpu/OpenGlElementBuffer.h"
#include "gpu/OpenGlFrameBuffer.h"
#include "gpu/OpenGlGraphicsContext.h"
#include "gpu/OpenGlRenderApi.h"
#include "gpu/OpenGlVertexArray.h"
#include "gpu/OpenGlVertexBuffer.h"
#include "gui/ImGuiApi.h"
#include "img/StbImage.hpp"
#include "input/GlfwInput.h"
#include "model/AssimpModelLoaderImpl.h"
#include "shader/OpenGlShader.h"
#include "shader/OpenGlShaderCompilerImpl.h"
#include "sl/core/Logger.h"
#include "sl/graphics/GraphicsContext.h"
#include "sl/graphics/ShaderCompilerImpl.h"
#include "sl/graphics/buffer/ElementBuffer.h"
#include "sl/platform/PlatformDetector.h"
#include "texture/OpenGlCubemap.h"
#include "texture/OpenGlTexture.h"
#include "window/GlfwWindow.h"

namespace sl::platform {

std::unique_ptr<graphics::RenderApi::Factory> createRenderApiFactory() {
    return std::make_unique<gpu::OpenGlRenderApiFactory>();
}

std::unique_ptr<geometry::ModelLoaderImpl::Factory> createModelLoaderImplFactory() {
    return std::make_unique<model::AssimpModelLoaderImplFactory>();
}

std::unique_ptr<graphics::ShaderCompilerImpl::Factory> createShaderCompilerImplFactory() {
    return std::make_unique<shader::OpenGlShaderCompilerImplFactory>();
}

std::unique_ptr<graphics::GraphicsContext::Factory> createGraphicsContextFactory() {
    return std::make_unique<gpu::OpenGlGraphicsContextFactory>();
}

std::unique_ptr<core::Window::Factory> createWindowFactory() {
    return std::make_unique<window::GlfwWindowFactory>();
}

std::unique_ptr<core::Input::Factory> createInputFactory() {
    return std::make_unique<input::GlfwInputFactory>();
}

std::unique_ptr<sl::gui::GuiApi::Factory> createGuiApiFactory() {
    return std::make_unique<sl::platform::gui::detail::ImGuiApiFactory>();
}

std::unique_ptr<graphics::Texture::Factory> createTextureFactory() {
    return std::make_unique<texture::OpenGlTextureFactory>();
}

std::unique_ptr<graphics::Cubemap::Factory> createCubemapFactory() {
    return std::make_unique<texture::OpenGlCubemapFactory>();
}

std::unique_ptr<graphics::buffer::VertexArray::Factory> createVertexArrayFactory() {
    return std::make_unique<gpu::OpenGlVertexArrayFactory>();
}

std::unique_ptr<graphics::buffer::ElementBuffer::Factory> createElementBufferFactory() {
    return std::make_unique<gpu::OpenGlElementBufferFactory>();
}

std::unique_ptr<graphics::buffer::VertexBuffer::Factory> createVertexBufferFactory() {
    return std::make_unique<gpu::OpenGlVertexBufferFactory>();
}

std::unique_ptr<graphics::buffer::FrameBuffer::Factory> createFrameBufferFactory() {
    return std::make_unique<gpu::OpenGlFrameBufferFactory>();
}

std::unique_ptr<graphics::Image::Factory> createImageFactory() {
    return std::make_unique<img::StbImageFactory>();
}

std::unique_ptr<graphics::Shader::Factory> createShaderFactory() {
    return std::make_unique<shader::OpenGlShaderFactory>();
}
}
