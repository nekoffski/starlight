#include "Cubemap.h"

#include <ranges>

#include "sl/async/AsyncManager.hpp"
#include "sl/gfx/Texture.h"

#include "TextureManager.h"

namespace sl::gfx {

void Cubemap::loadAsync(const CubemapArgs& paths, const std::string& name, std::unique_ptr<core::Output<Cubemap>> output) {
    class LoadAsync : public async::AsyncTask {
    public:
        explicit LoadAsync(const CubemapArgs& paths, const std::string& name, std::unique_ptr<core::Output<Cubemap>> output)
            : m_paths(paths)
            , m_name(name)
            , m_output(std::move(output)) {
        }

        void executeAsync() override {
            std::ranges::transform(m_paths, m_faces.begin(),
                [](const auto& face) -> std::unique_ptr<gfx::Image> { return gfx::TextureManager::get().loadImage(face); });
        }

        void finalize() override {
            auto cubemap = gfx::TextureManager::get().createCubemap(m_paths, m_name);
            cubemap->m_facesPaths = m_paths;

            m_output->set(std::move(cubemap));
        }

        std::string asString() const override {
            return fmt::format("AsyncLoadCubemap");
        }

    private:
        CubemapArgs m_paths;
        std::string m_name;
        std::unique_ptr<core::Output<Cubemap>> m_output;

        std::array<std::unique_ptr<gfx::Image>, facesCount> m_faces;
    };

    async::AsyncManager::get().executeAsyncTask<LoadAsync>(paths, name, std::move(output));
}
}