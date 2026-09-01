
#include "starlight/core/Core.hh"
#include "starlight/core/Log.hh"
#include "starlight/renderer/RenderingSystem.hh"

int main() {
    using namespace sl;

    log::init();
    log::info("Hello world!");

    RenderingSystem rs;

    return 0;
}
