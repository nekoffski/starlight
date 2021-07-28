#pragma once

#include "sl/core/Singleton.hpp"

#include "Cubemap.h"
#include "Image.h"
#include "Texture.h"

namespace sl::gfx {

class TextureManager : public core::Singleton<TextureManager> {
public:
    std::unique_ptr<Image> loadImage(const std::string& path, int desiredChannels = 0) {
        auto image = m_imageFactory->create(path, desiredChannels);
        image->m_path = path;

        return image;
    }

    std::unique_ptr<Cubemap> createCubemap(const CubemapArgs& args, const std::string& name) {
        std::array<std::unique_ptr<gfx::Image>, facesCount> faces;
        std::array<gfx::Image*, facesCount> facesView;

        std::ranges::transform(args, faces.begin(),
            [&](const auto& path) -> std::unique_ptr<Image> { return loadImage(path); });
        std::ranges::transform(faces, facesView.begin(), [](const auto& face) -> gfx::Image* { return face.get(); });

        auto cubemap = m_cubemapFactory->create(facesView);

        cubemap->m_facesPaths = args;
        cubemap->name = name;

        return cubemap;
    }

    std::unique_ptr<Cubemap> createOmnidirectionalShaderMap() {
        return m_cubemapFactory->create(Texture::shadowMapSize, Texture::shadowMapSize);
    }

    std::unique_ptr<Texture> createShadowMap() {
        return m_textureFactory->create(Texture::shadowMapSize, Texture::shadowMapSize);
    }

    std::unique_ptr<Texture> createTexture(const std::string& path, const std::string& name) {
        auto textureImage = loadImage(path);
        return createTexture(*textureImage, name);
    }

    std::unique_ptr<Texture> createTexture(gfx::Image& image, const std::string& name) {
        auto texture = m_textureFactory->create(image);

        texture->path = image.getPath();
        texture->name = name;

        return texture;
    }

    std::unique_ptr<Texture> createTexture(unsigned int width, unsigned int height,
        int internalFormat = STARL_DEPTH_COMPONENT, int format = STARL_DEPTH_COMPONENT) {

        return m_textureFactory->create(width, height, internalFormat, format);
    }

    void setTextureFactory(Texture::Factory* textureFactory) {
        m_textureFactory = textureFactory;
    }

    void setCubemapFactory(Cubemap::Factory* cubemapFactory) {
        m_cubemapFactory = cubemapFactory;
    }

    void setImageFactory(Image::Factory* imageFactory) {
        m_imageFactory = imageFactory;
    }

private:
    Texture::Factory* m_textureFactory;
    Cubemap::Factory* m_cubemapFactory;
    Image::Factory* m_imageFactory;
};
}