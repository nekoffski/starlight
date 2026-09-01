
#include <SDL3/SDL.h>

#include "starlight/core/Core.hh"
#include "starlight/core/Log.hh"
#include "starlight/core/Scope.hh"

int main() {
    using namespace sl;

    log::init();
    log::info("Hello world!");

    log::expect(
        SDL_Init(SDL_INIT_VIDEO) > 0, "Could not initialize SDL: {}",
        SDL_GetError()
    );
    auto window = SDL_CreateWindow("Starlight", 800, 600, 0);
    log::expect(window, "Could not create window");

    ON_SCOPE_EXIT {
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
