#pragma once

#include <memory>

#include "GraphicsContextMock.hpp"
#include "GuiApiMock.hpp"
#include "InputMock.hpp"
#include "ModelLoaderImplMock.hpp"
#include "RenderApiMock.hpp"
#include "ShaderCompilerImplMock.hpp"
#include "WindowMock.hpp"

using namespace sl;

inline void setupMockFactories() {
    graphics::ShaderCompilerImpl::factory = std::make_unique<ShaderCompilerImplMock::Factory>();
    graphics::GraphicsContext::factory = std::make_unique<GraphicsContextMock::Factory>();
    graphics::RenderApi::factory = std::make_unique<RenderApiMock::Factory>();
    core::Window::factory = std::make_unique<WindowMock::Factory>();
    core::Input::factory = std::make_unique<InputMock::Factory>();
    gui::GuiApi::factory = std::make_unique<GuiApiMock::Factory>();
    geometry::ModelLoaderImpl::factory = std::make_unique<ModelLoaderImplMock::Factory>();
}