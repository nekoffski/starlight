#pragma once

#include <memory>
#include <optional>

#include "sl/geom/ModelLoader.h"
#include "sl/gfx/Image.h"

#include "GPU.h"
#include "IO.h"

namespace sl::platform {

struct Platform {
    explicit Platform(IO&& io, GPU&& gpu, std::unique_ptr<gfx::Image::Factory> imageFactory, std::unique_ptr<geom::ModelLoader> modelLoader);

    class Builder {
    public:
        std::unique_ptr<Platform> build() &&;

        // clang-format off
        template <typename T, typename ...Args> requires std::derived_from<T, IO>
        Builder&& setIO(Args&& ...args) && {
            m_io = T{std::forward<Args>(args)...};
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
