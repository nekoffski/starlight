#pragma once

#include <SDL3/SDL.h>

#include <kcore/Concepts.hh>
#include <utility>

#include "starlight/core/Config.hh"
#include "starlight/event/EventBus.hh"
#include "starlight/window/Window.hh"

#if defined(SL_USE_METAL)
#include "starlight/platform/metal/MetalRenderSurfaceProvider.hh"

#elif defined(SL_USE_VULKAN)
#include "starlight/platform/vulkan/VulkanRenderSurfaceProvider.hh"

#else
#error "Unsupported platform"
#endif

namespace sl {

class SDLWindowBase : public Window {
   public:
    explicit SDLWindowBase(const Config& config, EventBus bus);
    ~SDLWindowBase() override;

    // Drains SDL's shared queue and routes events to their owning windows.
    void pollEvents() override;
    WindowId id() const override;

    bool isPressed(Key key) const override;
    bool isPressed(MouseButton button) const override;

    bool isFocused() const override;
    Vec2f mousePosition() const override;

   protected:
    void checkThread() const;

    SDL_Window* m_window{nullptr};

   private:
    void handleEvent(const SDL_Event& event);
    void handleKey(const SDL_KeyboardEvent& event);
    void handleMouseButton(const SDL_MouseButtonEvent& event);

    EventBus m_bus;
};

#if defined(SL_USE_METAL)

class SDLMetalWindow : public SDLWindowBase, public MetalRenderSurfaceProvider {
   public:
    explicit SDLMetalWindow(const Config& config, EventBus bus);
    ~SDLMetalWindow();

    CA::MetalLayer* getLayer() override;

   private:
    SDL_MetalView m_metalView{nullptr};
    CA::MetalLayer* m_metalLayer{nullptr};
};

using SDLWindow = SDLMetalWindow;

#elif defined(SL_USE_VULKAN)

class SDLVulkanWindow : public SDLWindowBase,
                        public VulkanRenderSurfaceProvider {
   public:
    explicit SDLVulkanWindow(const Config& config, EventBus bus)
        : SDLWindowBase(config, std::move(bus)) {}
    ~SDLVulkanWindow() {}
};

using SDLWindow = SDLVulkanWindow;

#else
#error "Unsupported platform"
#endif

}  // namespace sl
