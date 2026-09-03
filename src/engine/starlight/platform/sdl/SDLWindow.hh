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

class SDLWindow : public RenderSurfaceProviderImpl {
   public:
    explicit SDLWindow(const Config& config);
    ~SDLWindow();

   private:
#if defined(SL_USE_METAL)
    CA::MetalLayer* createLayer() override;
    void destroyLater(CA::MetalLayer* layer) override;
#else
#error "Unsupported platform"
#endif

    SDL_Window* m_window{nullptr};

    inline static std::atomic<u8> s_windowCount{0};
};

}  // namespace sl
