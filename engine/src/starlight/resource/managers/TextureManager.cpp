#include "TextureManager.h"

#include <kc/core/Log.h>
#include <fmt/core.h>

#include "starlight/core/math/Glm.h"
#include "starlight/renderer/Texture.h"
#include "starlight/renderer/gpu/TextureLoader.h"
#include "starlight/resource/resources/ImageData.h"

namespace sl {

TextureManager::TextureManager(TextureLoader& textureLoader) :
    m_textureLoader(textureLoader) {
    createDefaultTexture();
    createDefaultNormalMap();
    createDefaultSpecularMap();
}

Texture* TextureManager::load(const std::string& name) {
    LOG_TRACE("Loading texture '{}'", name);

    if (auto texture = m_textures.find(name); texture != m_textures.end()) {
        LOG_WARN(
          "Texture {} already loaded, returning pointer to the existing one", name
        );
        return texture->second.get();
    }

    const auto imageData = STBImageData::load(name);

    sl::Texture::Properties props{
        .width         = imageData->width,
        .height        = imageData->height,
        .channels      = imageData->channels,
        .isTransparent = imageData->isTransparent,
    };

    ASSERT(imageData, "Could not load image: {}", name);

    m_textures[name] = m_textureLoader.load(name, props, imageData->pixels);
    return m_textures[name].get();
}

Texture* TextureManager::acquire(const std::string& name) const {
    LOG_TRACE("Acquiring texture '{}'", name);
    if (auto texture = m_textures.find(name); texture != m_textures.end()) {
        return texture->second.get();
    } else {
        LOG_WARN("Texture {} not found", name);
        return nullptr;
    }
}

void setColor(uint32_t i, const Vec4f& color, std::vector<u8>& pixels) {
    pixels[i]     = color.x;
    pixels[i + 1] = color.y;
    pixels[i + 2] = color.z;
    pixels[i + 3] = color.w;
}

void TextureManager::createDefaultTexture() {
    Texture::Properties props{
        .width = 256, .height = 256, .channels = 4, .isTransparent = false
    };
    std::vector<u8> pixels(props.width * props.height * props.channels, 255);
    float scale = 8;

    for (uint32_t i = 0; i < pixels.size(); i += props.channels) {
        uint32_t x = (i / 4) % props.width;
        uint32_t y = std::floor((i / 4) / props.width);

        float xPattern = std::sin(((float)x / props.width) * 2.0f * M_PI * scale);
        float yPattern = std::cos(((float)y / props.height) * 2.0f * M_PI * scale);

        setColor(
          i,
          xPattern * yPattern > 0.0f
            ? Vec4f{ 0.0f }
            : Vec4f{ 255.0f, 0.0f, 0.0f, 255.0f },
          pixels
        );
    }
    m_textures[defaultTextureName] =
      m_textureLoader.load(defaultTextureName, props, pixels.data());
}

void TextureManager::createDefaultSpecularMap() {
    Texture::Properties props{
        .width = 16, .height = 16, .channels = 4, .isTransparent = false
    };
    std::vector<u8> pixels(props.width * props.height * props.channels, 0);
    m_textures[defaultSpecularMapName] =
      m_textureLoader.load(defaultSpecularMapName, props, pixels.data());
}

void TextureManager::createDefaultNormalMap() {
    Texture::Properties props{
        .width = 16, .height = 16, .channels = 4, .isTransparent = false
    };
    std::vector<u8> pixels(props.width * props.height * props.channels, 0);

    Vec4f zAxis{ 0.0f, 0.0f, 255.0f, 255.0f };
    for (int i = 0; i < pixels.size(); i += props.channels)
        setColor(i, zAxis, pixels);

    m_textures[defaultNormalMapName] =
      m_textureLoader.load(defaultNormalMapName, props, pixels.data());
}

Texture* TextureManager::getDefaultTexture() const {
    return m_textures.at(defaultTextureName).get();
}

Texture* TextureManager::getDefaultNormalMap() const {
    return m_textures.at(defaultNormalMapName).get();
}

Texture* TextureManager::getDefaultSpecularMap() const {
    return m_textures.at(defaultSpecularMapName).get();
}

void TextureManager::destroy(const std::string& name) {
    LOG_TRACE("Destroying texture '{}'", name);
    if (auto texture = m_textures.find(name); texture != m_textures.end()) [[likely]]
        m_textures.erase(texture);
    else
        LOG_WARN("Attempt to destroy not existing texture - {}, will ignore", name);
}

void TextureManager::destroyAll() { m_textures.clear(); }

}  // namespace sl