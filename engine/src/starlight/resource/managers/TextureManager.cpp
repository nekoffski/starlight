#include "TextureManager.h"

#include <kc/core/Log.h>
#include <fmt/core.h>

#include "starlight/core/math/Glm.h"
#include "starlight/renderer/Texture.h"
#include "starlight/renderer/gpu/TextureLoader.h"
#include "starlight/resource/resources/ImageData.h"

namespace sl {

TextureManager::TextureManager(
  TextureLoader& textureLoader, const ResourceLoader& resourceLoader
) :
    m_textureLoader(textureLoader),
    m_resourceLoader(resourceLoader) {
    loadDefaultTexture();
}

Texture* TextureManager::load(const std::string& name) {
    LOG_TRACE("Loading texture '{}'", name);

    if (auto texture = m_textures.find(name); texture != m_textures.end()) {
        LOG_WARN(
          "Texture {} already loaded, returning pointer to the existing one", name
        );
        return texture->second.get();
    }

    const auto imageData = m_resourceLoader.loadImageData(name);

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

void TextureManager::loadDefaultTexture() {
    Texture::Properties props{
        .width = 256, .height = 256, .channels = 4, .isTransparent = false
    };
    std::vector<uint8_t> pixels(props.width * props.height * props.channels, 255);

    float scale = 8;

    const auto setColor = [&](uint32_t i, const Vec3f& color) {
        pixels[i]     = color.x;
        pixels[i + 1] = color.y;
        pixels[i + 2] = color.z;
    };

    for (uint32_t i = 0; i < pixels.size(); i += 4) {
        uint32_t x = (i / 4) % props.width;
        uint32_t y = std::floor((i / 4) / props.width);

        float xPattern = std::sin(((float)x / props.width) * 2.0f * M_PI * scale);
        float yPattern = std::cos(((float)y / props.height) * 2.0f * M_PI * scale);

        setColor(
          i, xPattern * yPattern > 0.0f ? Vec3f{ 0.0f } : Vec3f{ 255.0f, 0.0f, 0.0f }
        );
    }

    m_defaultTexture =
      m_textureLoader.load("internal-default-texture", props, pixels.data());
}

Texture* TextureManager::getDefaultTexture() const { return m_defaultTexture.get(); }

void TextureManager::destroy(const std::string& name) {
    LOG_TRACE("Destroying texture '{}'", name);
    if (auto texture = m_textures.find(name); texture != m_textures.end()) [[likely]]
        m_textures.erase(texture);
    else
        LOG_WARN("Attempt to destroy not existing texture - {}, will ignore", name);
}

void TextureManager::destroyAll() { m_textures.clear(); }

}  // namespace sl