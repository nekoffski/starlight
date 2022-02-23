#include "Platform.hpp"

namespace sl::platform {

Platform::Platform(IO&& io, GPU&& gpu, std::unique_ptr<gfx::Image::Factory> imageFactory,
                   std::unique_ptr<geom::ModelLoader> modelLoader)
    : io(std::move(io)),
      gpu(std::move(gpu)),
      imageFactory(std::move(imageFactory)),
      modelLoader(std::move(modelLoader)) {}

std::unique_ptr<Platform> Platform::Builder::build() && {
    ASSERT(m_io.has_value(), "IO must be set");
    ASSERT(m_gpu.has_value(), "GPU must be set");
    ASSERT(m_imageFactory != nullptr, "Image factory must be set");
    ASSERT(m_modelLoader != nullptr, "Model loader must be set");

    return std::make_unique<Platform>(std::move(*m_io), std::move(*m_gpu),
                                      std::move(m_imageFactory), std::move(m_modelLoader));
}

}  // namespace sl::platform
