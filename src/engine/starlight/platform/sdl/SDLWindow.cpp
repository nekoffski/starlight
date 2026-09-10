#include "SDLWindow.hh"

#include "starlight/core/Log.hh"
#include "starlight/event/Events.hh"

namespace sl {

SDLWindowBase::SDLWindowBase(const Config& config, EventBus bus)
    : m_bus(std::move(bus)) {
    if (s_windowCount.fetch_add(1) == 0) {
        log::expect(
            SDL_Init(SDL_INIT_VIDEO) > 0, "Could not initialize SDL: {}",
            SDL_GetError()
        );
    }

    m_window = SDL_CreateWindow("Starlight", 800, 600, SDL_WINDOW_RESIZABLE);
    log::expect(
        m_window != nullptr, "Could not create window: {}", SDL_GetError()
    );

    log::trace("SDLWindow created");
}

SDLWindowBase::~SDLWindowBase() {
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }

    if (s_windowCount.fetch_sub(1) == 1) {
        SDL_Quit();
    }

    log::trace("SDLWindow destroyed");
}

void SDLWindowBase::pollEvents(u8 maxPolledEvents) {
    SDL_Event ev;

    for (u8 i = 0; i < maxPolledEvents && SDL_PollEvent(&ev); ++i) {
        switch (ev.type) {
            case SDL_EVENT_QUIT:
                m_bus.post<QuitRequestedEvent>("SDL Window quit requested");
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                m_bus.post<WindowResizedEvent>(
                    static_cast<u32>(ev.window.data1),
                    static_cast<u32>(ev.window.data2)
                );
                break;
        }
    }
}

#if defined(SL_USE_METAL)

SDLMetalWindow::SDLMetalWindow(const Config& config, EventBus bus)
    : SDLWindowBase(config, std::move(bus)) {
    m_metalView = SDL_Metal_CreateView(m_window);
    log::expect(
        m_metalView != nullptr, "Could not create Metal view: {}",
        SDL_GetError()
    );

    m_metalLayer =
        reinterpret_cast<CA::MetalLayer*>(SDL_Metal_GetLayer(m_metalView));
    log::expect(
        m_metalLayer != nullptr, "Could not get Metal layer: {}", SDL_GetError()
    );
}

SDLMetalWindow::~SDLMetalWindow() {
    if (m_metalView) {
        SDL_Metal_DestroyView(m_metalView);
    }
}

CA::MetalLayer* SDLMetalWindow::getLayer() { return m_metalLayer; }

#else
#error "Unsupported platform"
#endif

}  // namespace sl
