#include "Texture.h"

#include "sl/async/AsyncManager.hpp"

namespace sl::gfx {

std::unique_ptr<Texture> Texture::load(const std::string& path, const std::string& name) {
    auto textureImage = Image::load(path);
    auto texture = factory->create(*textureImage);

    texture->path = path;
    texture->name = name;

    return texture;
}

std::unique_ptr<Texture> Texture::createShadowMap() {
    return factory->create(shadowMapSize, shadowMapSize);
}

void Texture::loadAsync(const std::string& path, const std::string& name, std::unique_ptr<core::Output<Texture>> output) {
    class LoadAsync : public async::AsyncTask {
    public:
        explicit LoadAsync(const std::string& path, const std::string& name, std::unique_ptr<core::Output<Texture>> output)
            : m_path(path)
            , m_name(name)
            , m_output(std::move(output))
            , m_image(nullptr) {
        }

        void executeAsync() override {
            m_image = Image::load(m_path);
        }

        std::string asString() const override {
            return fmt::format("AsyncLoadTexture_{}", m_path);
        }

        void finalize() override {
            if (not m_image)
                return;

            auto texture = Texture::factory->create(*m_image);

            texture->path = m_path;
            texture->name = m_name;

            m_output->set(std::move(texture));
        }

    private:
        std::string m_path;
        std::string m_name;
        std::unique_ptr<core::Output<Texture>> m_output;
        std::unique_ptr<Image> m_image;
    };

    async::AsyncManager::get()->executeAsyncTask<LoadAsync>(path, name, std::move(output));
}

}