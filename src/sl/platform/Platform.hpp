#pragma once

#include <memory>
#include <optional>

#include "sl/geom/ModelLoader.h"
#include "sl/gfx/Image.h"

#include "GPU.h"
#include "IO.h"

namespace sl::platform {

struct Platform {
    explicit Platform(IO&& io, GPU&& gpu, std::unique_ptr<gfx::Image::Factory> imageFactory, std::unique_ptr<geom::ModelLoader> modelLoader)
        : io(std::move(io))
        , gpu(std::move(gpu))
        , imageFactory(std::move(imageFactory))
        , modelLoader(std::move(modelLoader)) {
    }

    class Builder {
    public:
        std::unique_ptr<Platform> build() && {
            SL_ASSERT(m_io.has_value(), "IO must be set");
            SL_ASSERT(m_gpu.has_value(), "GPU must be set");
            SL_ASSERT(m_imageFactory != nullptr, "Image factory must be set");
            SL_ASSERT(m_modelLoader != nullptr, "Model loader must be set");

            return std::make_unique<Platform>(
                std::move(*m_io), std::move(*m_gpu), std::move(m_imageFactory), std::move(m_modelLoader));
        }

        // clang-format off
        template <typename T> requires std::derived_from<T, IO>
        Builder&& setIO() && {
            m_io = T{};
            return std::move(*this);
        }

        template <typename T> requires std::derived_from<T, GPU>
        Builder&& setGPU() && {
            m_gpu = T{};
            return std::move(*this);
        }

        template <typename T> requires std::derived_from<T, gfx::Image::Factory>
        Builder&& setImageFactory() && {
            m_imageFactory = std::make_unique<T>();
            return std::move(*this);
        }

        template <typename T> requires std::derived_from<T, geom::ModelLoader>
        Builder&& setModelLoader() && {
            m_modelLoader = std::make_unique<T>();
            return std::move(*this);
        }
        // clang-format on

    private:
        std::optional<IO> m_io;
        std::optional<GPU> m_gpu;
        std::unique_ptr<gfx::Image::Factory> m_imageFactory = nullptr;
        std::unique_ptr<geom::ModelLoader> m_modelLoader = nullptr;
    };

    IO io;
    GPU gpu;

    std::unique_ptr<gfx::Image::Factory> imageFactory;
    std::unique_ptr<geom::ModelLoader> modelLoader;
};

}
