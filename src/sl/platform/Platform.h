#pragma once

#include <memory>

#include "sl/core/Input.h"
#include "sl/core/Window.h"
#include "sl/geometry/ModelLoaderImpl.h"
#include "sl/graphics/Cubemap.h"
#include "sl/graphics/GraphicsContext.h"
#include "sl/graphics/Image.h"
#include "sl/graphics/RenderApi.h"
#include "sl/graphics/Shader.h"
#include "sl/graphics/ShaderCompilerImpl.h"
#include "sl/graphics/Texture.h"
#include "sl/graphics/buffer/ElementBuffer.h"
#include "sl/graphics/buffer/FrameBuffer.h"
#include "sl/graphics/buffer/VertexArray.h"
#include "sl/graphics/buffer/VertexBuffer.h"
#include "sl/gui/GuiApi.h"

namespace sl::platform {
std::unique_ptr<graphics::buffer::VertexArray::Factory> createVertexArrayFactory();
std::unique_ptr<graphics::buffer::ElementBuffer::Factory> createElementBufferFactory();
std::unique_ptr<graphics::buffer::VertexBuffer::Factory> createVertexBufferFactory();
std::unique_ptr<graphics::buffer::FrameBuffer::Factory> createFrameBufferFactory();
std::unique_ptr<graphics::Image::Factory> createImageFactory();
std::unique_ptr<graphics::Texture::Factory> createTextureFactory();
std::unique_ptr<graphics::Cubemap::Factory> crateCubemapFactory();
std::unique_ptr<graphics::Shader::Factory> createShaderFactory();
std::unique_ptr<core::Window::Factory> createWindowFactory();
std::unique_ptr<core::Input::Factory> createInputFactory();
std::unique_ptr<sl::gui::GuiApi::Factory> createGuiApiFactory();
std::unique_ptr<graphics::GraphicsContext::Factory> createGraphicsContextFactory();
std::unique_ptr<graphics::ShaderCompilerImpl::Factory> createShaderCompilerImplFactory();
std::unique_ptr<geometry::ModelLoaderImpl::Factory> createModelLoaderImplFactory();
std::unique_ptr<graphics::RenderApi::Factory> createRenderApiFactory();
}
