#pragma once

#include <SDL3/SDL.h>

#include <atomic>

#include "starlight/core/Concepts.hh"
#include "starlight/core/Config.hh"

#if defined(SL_USE_METAL)
#include "starlight/platform/metal/MetalRenderSurfaceProvider.hh"

using RenderSurfaceProviderImpl = sl::MetalRenderSurfaceProvider;

#else
#error "Unsupported platform"
#endif

namespace sl {

class SDLWindowBase : public virtual NonCopyable, public virtual NonMovable {
   public:
    explicit SDLWindowBase(const Config& config);
    ~SDLWindowBase();

   protected:
    SDL_Window* m_window{nullptr};

    inline static std::atomic<u8> s_windowCount{0};
};

#if defined(SL_USE_METAL)

class SDLMetalWindow : public SDLWindowBase, public MetalRenderSurfaceProvider {
   public:
    explicit SDLMetalWindow(const Config& config);
    ~SDLMetalWindow();

    CA::MetalLayer* getLayer() override;

   private:
    SDL_MetalView m_metalView{nullptr};
    CA::MetalLayer* m_metalLayer{nullptr};
};

// class SDLWindow : public RenderSurfaceProviderImpl {
//    public:
//     explicit SDLWindow(const Config& config);
//     ~SDLWindow();

//    private:

// };

using SDLWindow = SDLMetalWindow;

#else
#error "Unsupported platform"
#endif

}  // namespace sl
