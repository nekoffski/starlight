#pragma once

#include <memory>

#include "sl/core/Input.h"
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
#include "sl/gfx/buffer/VertexArray.h"
#include "sl/gfx/buffer/VertexBuffer.h"
#include "sl/gui/GuiApi.h"

namespace sl::platform {

std::unique_ptr<gfx::buffer::VertexArray::Factory> createVertexArrayFactory();
std::unique_ptr<gfx::buffer::ElementBuffer::Factory> createElementBufferFactory();
std::unique_ptr<gfx::buffer::VertexBuffer::Factory> createVertexBufferFactory();
std::unique_ptr<gfx::buffer::FrameBuffer::Factory> createFrameBufferFactory();
std::unique_ptr<gfx::Image::Factory> createImageFactory();
std::unique_ptr<gfx::Texture::Factory> createTextureFactory();
std::unique_ptr<gfx::Cubemap::Factory> createCubemapFactory();
std::unique_ptr<gfx::Shader::Factory> createShaderFactory();
std::unique_ptr<core::Window::Factory> createWindowFactory();
std::unique_ptr<core::Input::Factory> createInputFactory();
std::unique_ptr<sl::gui::GuiApi::Factory> createGuiApiFactory();
std::unique_ptr<gfx::GraphicsContext::Factory> createGraphicsContextFactory();
std::unique_ptr<gfx::ShaderCompilerImpl::Factory> createShaderCompilerImplFactory();
std::unique_ptr<geom::ModelLoaderImpl::Factory> createModelLoaderImplFactory();
std::unique_ptr<gfx::RenderApi::Factory> createRenderApiFactory();
}
