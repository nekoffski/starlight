#include "Window.hh"

namespace sl {

u32 Window::getFramebufferWidth() const { return getFramebufferSize().x; }
u32 Window::getFramebufferHeight() const { return getFramebufferSize().y; }

u32 Window::getWidth() const { return getSize().x; }
u32 Window::getHeight() const { return getSize().y; }

}  // namespace sl
