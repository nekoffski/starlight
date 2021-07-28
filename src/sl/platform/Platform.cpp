#include "Platform.h"

#include <memory>

#include "gl/OpenGlCubemap.h"
#include "gl/OpenGlElementBuffer.h"
#include "gl/OpenGlFrameBuffer.h"
#include "gl/OpenGlGraphicsContext.h"
#include "gl/OpenGlRenderApi.h"
#include "gl/OpenGlRenderBuffer.h"
#include "gl/OpenGlShader.h"
#include "gl/OpenGlShaderCompilerImpl.h"
#include "gl/OpenGlTexture.h"
#include "gl/OpenGlVertexArray.h"
#include "gl/OpenGlVertexBuffer.h"
#include "glfw/GlfwWindow.h"
#include "gui/ImGuiApi.h"
#include "model/AssimpModelLoaderImpl.h"
#include "sl/core/Logger.h"
#include "sl/platform/PlatformDetector.h"
#include "stb/StbImage.hpp"

namespace sl::platform {

std::unique_ptr<gfx::RenderBuffer::Factory> createRenderBufferFactory() {
    return std::make_unique<gl::OpenGlRenderBufferFactory>();
}

std::unique_ptr<gfx::RenderApi::Factory> createRenderApiFactory() {
    return std::make_unique<gl::OpenGlRenderApiFactory>();
}

std::unique_ptr<geom::ModelLoaderImpl::Factory> createModelLoaderImplFactory() {
    return std::make_unique<model::AssimpModelLoaderImplFactory>();
}

std::unique_ptr<gfx::ShaderCompilerImpl::Factory> createShaderCompilerImplFactory() {
    return std::make_unique<gl::OpenGlShaderCompilerImplFactory>();
}

std::unique_ptr<gfx::GraphicsContext::Factory> createGraphicsContextFactory() {
    return std::make_unique<gl::OpenGlGraphicsContextFactory>();
}

std::unique_ptr<core::Window::Factory> createWindowFactory() {
    return std::make_unique<glfw::GlfwWindowFactory>();
}

std::unique_ptr<sl::gui::GuiApi::Factory> createGuiApiFactory() {
    return std::make_unique<sl::platform::gui::ImGuiApiFactory>();
}

std::unique_ptr<gfx::Texture::Factory> createTextureFactory() {
    return std::make_unique<gl::OpenGlTextureFactory>();
}

std::unique_ptr<gfx::Cubemap::Factory> createCubemapFactory() {
    return std::make_unique<gl::OpenGlCubemapFactory>();
}

std::unique_ptr<gfx::VertexArray::Factory> createVertexArrayFactory() {
    return std::make_unique<gl::OpenGlVertexArrayFactory>();
}

std::unique_ptr<gfx::ElementBuffer::Factory> createElementBufferFactory() {
    return std::make_unique<gl::OpenGlElementBufferFactory>();
}

std::unique_ptr<gfx::VertexBuffer::Factory> createVertexBufferFactory() {
    return std::make_unique<gl::OpenGlVertexBufferFactory>();
}

std::unique_ptr<gfx::FrameBuffer::Factory> createFrameBufferFactory() {
    return std::make_unique<gl::OpenGlFrameBufferFactory>();
}

std::unique_ptr<gfx::Image::Factory> createImageFactory() {
    return std::make_unique<stb::StbImageFactory>();
}

std::unique_ptr<gfx::Shader::Factory> createShaderFactory() {
    return std::make_unique<gl::OpenGlShaderFactory>();
}
}
