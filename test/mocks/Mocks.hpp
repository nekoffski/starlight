#pragma once

#include <memory>

#include "ElementBufferMock.hpp"
#include "FrameBufferMock.hpp"
#include "GraphicsContextMock.hpp"
#include "GuiApiMock.hpp"
#include "InputMock.hpp"
#include "ModelLoaderImplMock.hpp"
#include "RenderApiMock.hpp"
#include "ShaderCompilerImplMock.hpp"
#include "ShaderMock.hpp"
#include "VertexArrayMock.hpp"
#include "VertexBufferMock.hpp"
#include "WindowMock.hpp"

#include "sl/core/Clock.h"
#include "sl/platform/core/StdClockImpl.h"

#include "sl/gfx/ShaderCompiler.hpp"

using namespace sl;
using namespace sl::gfx;
using namespace sl::core;
using namespace sl::gui;
using namespace sl::geom;
using namespace sl::gfx::buffer;

namespace {

template <typename Original, typename Mock>
void setupMockFactory() {
    Original::factory = std::make_unique<typename Mock::Factory>();
    Mock::Factory::instance = static_cast<typename Mock::Factory*>(Original::factory.get());
}

template <typename T>
void resetMockFactory() {
    T::factory.reset();
}

template <typename T>
void safeReset(std::unique_ptr<T>& ptr) {
    if (ptr)
        ptr.reset();
}

#define SETUP_MOCK(Class) setupMockFactory<Class, Class##Mock>()

}

struct Mocks {
    std::shared_ptr<WindowMock> windowMock = std::make_shared<WindowMock>();
    std::shared_ptr<InputMock> inputMock = std::make_shared<InputMock>();
    std::shared_ptr<GraphicsContextMock> graphicsContextMock = std::make_shared<GraphicsContextMock>();
    std::shared_ptr<GuiApiMock> guiApiMock = std::make_shared<GuiApiMock>();
    std::unique_ptr<ModelLoaderImplMock> modelLoaderImplMock = std::make_unique<ModelLoaderImplMock>();
    std::unique_ptr<ShaderCompilerImplMock> shaderCompilerImplMock = std::make_unique<ShaderCompilerImplMock>();
    std::unique_ptr<RenderApiMock> renderApiMock = std::make_unique<RenderApiMock>();
    std::shared_ptr<ShaderMock> shaderMock = std::make_shared<ShaderMock>();
    std::shared_ptr<VertexArrayMock> vertexArrayMock = std::make_shared<VertexArrayMock>();
    std::shared_ptr<VertexBufferMock> vertexBufferMock = std::make_shared<VertexBufferMock>();
    std::shared_ptr<ElementBufferMock> elementBufferMock = std::make_shared<ElementBufferMock>();
    std::shared_ptr<FrameBufferMock> frameBufferMock = std::make_shared<FrameBufferMock>();
};

inline void setupMockOnCalls(Mocks& mocks) {
}

inline void setupMockFactories() {
    SETUP_MOCK(ShaderCompilerImpl);
    SETUP_MOCK(GraphicsContext);
    SETUP_MOCK(RenderApi);
    SETUP_MOCK(Window);
    SETUP_MOCK(Input);
    SETUP_MOCK(GuiApi);
    SETUP_MOCK(ModelLoaderImpl);
    SETUP_MOCK(VertexArray);
    SETUP_MOCK(VertexBuffer);
    SETUP_MOCK(ElementBuffer);
    SETUP_MOCK(Shader);
    SETUP_MOCK(FrameBuffer);

    CLOCK().setClockImpl<platform::core::StdClockImpl>();
}

inline void resetMockFactories() {
    resetMockFactory<GraphicsContext>();
    resetMockFactory<RenderApi>();
    resetMockFactory<Window>();
    resetMockFactory<Input>();
    resetMockFactory<GuiApi>();
    resetMockFactory<ModelLoaderImpl>();
    resetMockFactory<VertexArray>();
    resetMockFactory<VertexBuffer>();
    resetMockFactory<ElementBuffer>();
    resetMockFactory<Shader>();
    resetMockFactory<ShaderCompilerImpl>();
    resetMockFactory<FrameBuffer>();

    safeReset(ShaderCompiler::impl);
}