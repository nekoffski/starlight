#include "SDLWindow.hh"

#include "starlight/core/Log.hh"

namespace sl {

SDLWindow::SDLWindow(const Config& config) {
    if (s_windowCount.fetch_add(1) == 0) {
        log::expect(
            SDL_Init(SDL_INIT_VIDEO) > 0, "Could not initialize SDL: {}",
            SDL_GetError()
        );
    }

    m_window = SDL_CreateWindow("Starlight", 800, 600, 0);
    log::expect(
        m_window != nullptr, "Could not create window: {}", SDL_GetError()
    );

    log::trace("SDLWindow created");
}

SDLWindow::~SDLWindow() {
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }

    if (s_windowCount.fetch_sub(1) == 1) {
        SDL_Quit();
    }

    log::trace("SDLWindow destroyed");
}

#if defined(SL_USE_METAL)

CA::MetalLayer* SDLWindow::createLayer() {
    return reinterpret_cast<CA::MetalLayer*>(SDL_Metal_GetLayer(m_window));
}

void SDLWindow::destroyLater(CA::MetalLayer* layer) {
    if (layer) {
        SDL_Metal_DestroyView(reinterpret_cast<SDL_MetalView>(layer));
    }
}

#else
#error "Unsupported platform"
#endif

}  // namespace sl
