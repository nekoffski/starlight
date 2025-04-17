#include "Window.hh"

#include "Events.hh"

#ifdef SL_USE_GLFW
#include "glfw/GLFWWindow.hh"
#endif

namespace sl {

static Rect2<u32> calculateWorldViewport(u32 w, u32 h, const Vec4<f32>& scale) {
    return Rect2<u32>{
        Vec2<u32>(static_cast<u32>(w * scale.x), 0u),
        Vec2<u32>(static_cast<u32>(w * scale.z), static_cast<u32>(h * scale.w))
    };
}

static Rect2<u32> calculateUserInterfaceViewport(u32 w, u32 h) {
    return Rect2<u32>{
        .offset = Vec2<u32>{ 0u, 0u },
        .size   = Vec2<u32>{ w,  h  },
    };
}

Window::Window(kstd::UniquePtr<Impl> impl)
    : m_impl(std::move(impl))
    , m_userInterfaceLayoutScale(0.0f, 0.0f, 1.0f, 1.0f) {
    setCallbacks();
}

void Window::setCallbacks() {
    m_impl->onWindowCloseCallback([]() {
        log::info("Window closed, emitting event");
        EventProxy::get().emit<QuitEvent>("Window closed");
    });

    m_impl->onWindowResizeCallback(
      [&scale = m_userInterfaceLayoutScale](u32 width, u32 height) {
          log::trace("Window resized ({} x {}), emitting events", width, height);

          EventProxy::get().emit<WindowResized>(Vec2<u32>{ width, height });
          EventProxy::get().emit<ViewportChanged>(
            calculateUserInterfaceViewport(width, height),
            calculateWorldViewport(width, height, scale)
          );
      }
    );
}

Vec2<u32> Window::getSize() const { return m_impl->getSize(); }

Vec2<u32> Window::getFramebufferSize() const { return m_impl->getFramebufferSize(); }

void* Window::getHandle() { return m_impl->getHandle(); }

Window::Impl& Window::getImpl() { return *m_impl; }

u32 Window::getFramebufferWidth() const { return getFramebufferSize().x; }
u32 Window::getFramebufferHeight() const { return getFramebufferSize().y; }

u32 Window::getWidth() const { return getSize().x; }
u32 Window::getHeight() const { return getSize().y; }

Rect2<u32> Window::getWorldViewport() {
    const auto size = getFramebufferSize();
    return calculateWorldViewport(size.w, size.h, m_userInterfaceLayoutScale);
}

Rect2<u32> Window::getUserInterfaceViewport() {
    const auto size = getFramebufferSize();
    return calculateUserInterfaceViewport(size.w, size.h);
}

void Window::setUserInterfaceLayoutScale(const Vec4<f32>& scale) {
    m_userInterfaceLayoutScale = scale;
}

kstd::UniquePtr<Window::Impl> Window::Impl::create() {
    const auto& windowConfig = Globals::get().getConfig().window;
#ifdef SL_USE_GLFW
    return kstd::makeUnique<glfw::GLFWWindow>(windowConfig);
#else
    log::panic("Window provider not specified");
#endif
}

}  // namespace sl
