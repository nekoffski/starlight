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
    gfx::ShaderCompilerImpl::factory = std::make_unique<ShaderCompilerImplMock::Factory>();
    gfx::GraphicsContext::factory = std::make_unique<GraphicsContextMock::Factory>();
    gfx::RenderApi::factory = std::make_unique<RenderApiMock::Factory>();
    core::Window::factory = std::make_unique<WindowMock::Factory>();
    core::Input::factory = std::make_unique<InputMock::Factory>();
    gui::GuiApi::factory = std::make_unique<GuiApiMock::Factory>();
    geom::ModelLoaderImpl::factory = std::make_unique<ModelLoaderImplMock::Factory>();
}