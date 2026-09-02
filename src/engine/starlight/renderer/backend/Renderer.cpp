#include "Renderer.hh"

namespace sl {

Renderer::Renderer(const Config& config, RenderDevice& device)
    : m_config(config), m_device(device) {}

void Renderer::tick() {}

void Renderer::flush() {
    while (hasPendingWork()) {
        tick();
    }
}

bool Renderer::hasPendingWork() const { return false; }

}  // namespace sl
