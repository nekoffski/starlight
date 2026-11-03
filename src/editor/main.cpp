
#include <SDL3/SDL.h>

#include <kcore/Functional.hh>

#include "starlight/core/Core.hh"

int main() {
    using namespace sl;

    log::init({
        .formatPattern = "%^[%d-%m-%Y %T] [Th: %t] %-7l [starlight]: %v %$",
    });
    log::info("Hello world!");

    log::expect(
        SDL_Init(SDL_INIT_VIDEO) > 0, "Could not initialize SDL: {}",
        SDL_GetError()
    );
    auto window = SDL_CreateWindow("Starlight", 800, 600, 0);
    log::expect(window, "Could not create window");

    KCORE_DEFER {
        SDL_DestroyWindow(window);
        SDL_Quit();
    };

    SDL_Event event;
    bool running{true};

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
    }

    return 0;
}
