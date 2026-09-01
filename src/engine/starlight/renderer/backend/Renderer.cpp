#include "Renderer.hh"

namespace sl {

Renderer::Renderer(const Config& config, RenderDevice& device)
    : m_config(config), m_device(device) {}

}  // namespace sl
