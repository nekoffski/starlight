#include "Application.h"

#include "sl/core/Input.h"
#include "sl/core/Profiler.h"
#include "sl/event/Categories.h"
#include "sl/event/Emitter.hpp"
#include "sl/event/Event.h"
#include "sl/geometry/ModelLoader.hpp"
#include "sl/geometry/ModelLoaderImpl.h"
#include "sl/graphics/Cubemap.h"
#include "sl/graphics/GraphicsContext.h"
#include "sl/graphics/Image.h"
#include "sl/graphics/RenderApi.h"
#include "sl/graphics/Renderer.hpp"
#include "sl/graphics/ShaderCompiler.hpp"
#include "sl/graphics/ShaderCompilerImpl.h"
#include "sl/graphics/ViewFrustum.h"
#include "sl/graphics/buffer/ElementBuffer.h"
#include "sl/graphics/buffer/FrameBuffer.h"
#include "sl/graphics/buffer/VertexArray.h"
#include "sl/graphics/buffer/VertexBuffer.h"
#include "sl/gui/GuiApi.h"
#include "sl/platform/Platform.h"
#include "sl/utils/Globals.h"

namespace sl::app {

void Application::initDefaultFactories() {
    graphics::Image::factory = platform::createImageFactory();

    graphics::buffer::VertexArray::factory = platform::createVertexArrayFactory();
    graphics::buffer::VertexBuffer::factory = platform::createVertexBufferFactory();
    graphics::buffer::FrameBuffer::factory = platform::createFrameBufferFactory();
    graphics::buffer::ElementBuffer::factory = platform::createElementBufferFactory();

    graphics::Texture::factory = platform::createTextureFactory();
    graphics::Cubemap::factory = platform::createCubemapFactory();
    graphics::Shader::factory = platform::createShaderFactory();
    graphics::ShaderCompilerImpl::factory = platform::createShaderCompilerImplFactory();
    graphics::GraphicsContext::factory = platform::createGraphicsContextFactory();
    graphics::RenderApi::factory = platform::createRenderApiFactory();

    core::Window::factory = platform::createWindowFactory();
    core::Input::factory = platform::createInputFactory();

    gui::GuiApi::factory = platform::createGuiApiFactory();

    geometry::ModelLoaderImpl::factory = platform::createModelLoaderImplFactory();
}

Application::Application()
    : m_eventEmitter(m_eventEngine.createEmitter()) {
    event::Emitter::init(m_eventEmitter->clone());
}

void Application::init() {
    SL_INFO("Creating and initializing window instance.");
    auto windowSize = core::Window::Size { 1600, 900 };
    m_window = core::Window::factory->create(windowSize, "Starlight");
    m_window->init();
    m_window->setResizeCallback([](int width, int height) {
        SL_DEBUG("Window resized to: {}/{}", width, height);
        event::Emitter::m_xventEmitter->emit<event::WindowResizedEvent>(width, height);
    });

    auto windowHandle = m_window->getHandle();

    SL_INFO("Creating input instance.");
    m_input = core::Input::factory->create(windowHandle);

    SL_INFO("Creating graphics context instance.");
    m_graphicsContext = graphics::GraphicsContext::factory->create(windowHandle);

    auto viewport = graphics::ViewFrustum::Viewport { windowSize.width, windowSize.height };

    SL_INFO("Creating low level renderer instance.");
    m_lowLevelRenderer = std::make_shared<graphics::LowLevelRenderer>(m_graphicsContext,
        graphics::RenderApi::factory->create(), viewport);

    SL_INFO("Creating GUI API instance.");
    m_guiApi = gui::GuiApi::factory->create(windowHandle);

    SL_INFO("Creating model loader impl instance.");
    geometry::ModelLoader::impl = geometry::ModelLoaderImpl::factory->create();

    SL_INFO("Creating shader compiler impl instance.");
    graphics::ShaderCompiler::impl = graphics::ShaderCompilerImpl::factory->create();

    SL_INFO("Initializing global utils.");
    GLOBALS().init();

    SL_INFO("Creating renderer instance.");
    m_renderer = std::make_shared<graphics::Renderer>(m_lowLevelRenderer);

    SL_INFO("Creating scene systems instance");
    m_sceneSystems = std::make_shared<scene::SceneSystems>();
}

void Application::update(float deltaTime, float time) {
    SL_PROFILE_FUNCTION();

    m_window->update(deltaTime);
    m_context->update(*m_sceneSystems, deltaTime, time);

    m_eventEngine.spreadEvents();

    m_input->update();
}

void Application::render() {
    SL_PROFILE_FUNCTION();

    m_lowLevelRenderer->clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);
    m_context->render(*m_renderer);
    renderGui();
    m_lowLevelRenderer->swapBuffers();
}

void Application::renderGui() {
    m_guiApi->begin();
    m_context->renderGui(*m_guiApi);
    m_guiApi->end();
}

void Application::handleInput() {
    GLOBALS().flags.disableKeyboardInput = m_guiApi->isCapturingKeyboard();
    GLOBALS().flags.disableMouseInput = m_guiApi->isCapturingMouse();

    m_context->handleInput(*m_input);

    if (m_input->isMouseButtonPressed(STARL_MOUSE_BUTTON_MIDDLE))
        m_window->disableCursor();
    else
        m_window->enableCursor();
}

std::shared_ptr<ApplicationContext> Application::getActiveContext() const {
    return m_context;
}

void Application::switchContext(std::shared_ptr<ApplicationContext> context) {
    if (m_context)
        m_context->onDetach();
    m_context = context;
    m_context->onAttach();
}

bool Application::isRunning() const {
    return not m_window->getShouldClose();
}

void Application::forceStop() {
    m_window->setShouldClose(true);
}
}
