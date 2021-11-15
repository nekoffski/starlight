#include "Texture.h"

#include "TextureManager.h"
#include "sl/async/AsyncManager.hpp"

namespace sl::gfx {

// std::unique_ptr<Texture> Texture::load(const std::string& path, const std::string& name) {
//     auto textureImage = TextureManager::get().loadImage(path);
//     auto texture = factory->create(*textureImage);

//     texture->path = path;
//     texture->name = name;

//     return texture;
// }

// std::unique_ptr<Texture> Texture::createShadowMap() {
//     return factory->create(shadowMapSize, shadowMapSize);
// }

std::unique_ptr<Texture> Texture::clone() {
    return TextureManager::get().createTexture(
        getWidth(), getHeight(), internalFormat, format);
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
            m_image = TextureManager::get().loadImage(m_path);
        }

        std::string asString() const override {
            return fmt::format("AsyncLoadTexture_{}", m_path);
        }

        void finalize() override {
            if (not m_image)
                return;

            auto texture = TextureManager::get().createTexture(*m_image, m_name);

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

    async::AsyncManager::get().callAsync<LoadAsync>(path, name, std::move(output));
}

}