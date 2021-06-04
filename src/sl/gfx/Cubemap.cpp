#include "Cubemap.h"

#include <ranges>

#include "sl/async/AsyncEngine.hpp"

namespace sl::gfx {

std::unique_ptr<Cubemap> Cubemap::load(const CubemapArgs& paths, const std::string& name) {
    std::array<std::unique_ptr<gfx::Image>, facesCount> faces;
    std::array<gfx::Image*, facesCount> facesView;

    std::ranges::transform(paths, faces.begin(),
        [](const auto& path) -> std::unique_ptr<gfx::Image> { return gfx::Image::load(path); });
    std::ranges::transform(faces, facesView.begin(), [](const auto& face) -> gfx::Image* { return face.get(); });

    auto cubemap = factory->create(facesView);
    cubemap->m_facesPaths = paths;
    cubemap->name = name;

    return cubemap;
}

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
                [](const auto& face) -> std::unique_ptr<gfx::Image> { return gfx::Image::load(face); });
        }

        void finalize() override {
            std::array<gfx::Image*, facesCount> facesView;
            std::ranges::transform(m_faces, facesView.begin(), [](const auto& face) -> gfx::Image* { return face.get(); });

            auto cubemap = factory->create(facesView);
            cubemap->m_facesPaths = m_paths;
            cubemap->name = m_name;

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

    ASYNC_ENGINE().executeAsyncTask<LoadAsync>(paths, name, std::move(output));
}
}