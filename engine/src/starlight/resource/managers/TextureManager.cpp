#include "TextureManager.h"

#include <kc/core/Log.h>
#include <fmt/core.h>

#include "starlight/core/math/Glm.h"
#include "starlight/renderer/gpu/Texture.h"
#include "starlight/renderer/gpu/ResourcePools.h"
#include "starlight/resource/resources/ImageData.h"

namespace sl {

TextureManager::TextureManager(ResourcePools& resourcePools) :
    m_resourcePools(resourcePools), m_defaultTexture(nullptr),
    m_defaultNormalMap(nullptr), m_defaultSpecularMap(nullptr) {
    LOG_TRACE("Creating TextureManager");
    createDefaultTexture();
    createDefaultNormalMap();
    createDefaultSpecularMap();
    createDefaultTextureMap();
}

TextureManager::~TextureManager() {
    LOG_TRACE("Destroying TextureManager");
    m_resourcePools.destroyTextureMap(*TextureMap::defaultMap);
    destroyAll();
}

Texture* TextureManager::load(const std::string& name) {
    LOG_DEBUG("Loading texture '{}'", name);

    if (auto texture = m_textures.find(name); texture != m_textures.end()) {
        LOG_INFO(
          "Texture {} already loaded, returning pointer to the existing one", name
        );
        return texture->second;
    }

    const auto imageData = STBImageData::load(name);

    sl::Texture::Properties props{
        .width         = imageData->width,
        .height        = imageData->height,
        .channels      = imageData->channels,
        .isTransparent = imageData->isTransparent,
        .isWritable    = false,
        .name          = name
    };

    ASSERT(imageData, "Could not load image: {}", name);

    m_textures[name] = m_resourcePools.createTexture(
      props,
      std::span<u8>(
        imageData->pixels, imageData->width * imageData->height * imageData->channels
      )
    );
    return m_textures[name];
}

Texture* TextureManager::acquire(const std::string& name) const {
    LOG_DEBUG("Acquiring texture '{}'", name);
    if (auto texture = m_textures.find(name); texture != m_textures.end()) {
        return texture->second;
    } else {
        LOG_WARN("Texture {} not found", name);
        return nullptr;
    }
}

static void setColor(uint32_t i, const Vec4f& color, std::vector<u8>& pixels) {
    pixels[i]     = color.x;
    pixels[i + 1] = color.y;
    pixels[i + 2] = color.z;
    pixels[i + 3] = color.w;
}

void TextureManager::createDefaultTextureMap() {
    TextureMap::defaultMap = m_resourcePools.createTextureMap(
      TextureMap::Properties{ TextureMap::Use::diffuseMap }, *Texture::defaultDiffuse
    );
}

void TextureManager::createDefaultTexture() {
    LOG_TRACE("Creating default texture");
    Texture::Properties props{
        .width         = 256,
        .height        = 256,
        .channels      = 4,
        .isTransparent = false,
        .isWritable    = false,
        .name          = "DefaultTexture"
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
    m_textures[defaultTextureName] = m_resourcePools.createTexture(props, pixels);
    Texture::defaultDiffuse        = m_textures[defaultTextureName];
    LOG_TRACE("Default texture created");
}

void TextureManager::createDefaultSpecularMap() {
    LOG_TRACE("Creating default specular map");
    Texture::Properties props{
        .width         = 16,
        .height        = 16,
        .channels      = 4,
        .isTransparent = false,
        .isWritable    = false,
        .name          = defaultSpecularMapName
    };
    std::vector<u8> pixels(props.width * props.height * props.channels, 0);
    m_textures[defaultSpecularMapName] =
      m_resourcePools.createTexture(props, pixels);
    Texture::defaultSpecular = m_textures[defaultSpecularMapName];
    LOG_TRACE("Default specular map created");
}

void TextureManager::createDefaultNormalMap() {
    LOG_TRACE("Creating default normal map");
    Texture::Properties props{
        .width         = 16,
        .height        = 16,
        .channels      = 4,
        .isTransparent = false,
        .isWritable    = false,
        .name          = defaultNormalMapName
    };
    std::vector<u8> pixels(props.width * props.height * props.channels, 0);

    Vec4f zAxis{ 0.0f, 0.0f, 255.0f, 255.0f };
    for (int i = 0; i < pixels.size(); i += props.channels)
        setColor(i, zAxis, pixels);

    m_textures[defaultNormalMapName] = m_resourcePools.createTexture(props, pixels);
    Texture::defaultNormal           = m_textures[defaultNormalMapName];
    LOG_TRACE("Default specular normal map created");
}

void TextureManager::destroy(const std::string& name) {
    LOG_DEBUG("Destroying texture '{}'", name);
    if (auto texture = m_textures.find(name); texture != m_textures.end())
      [[likely]] {
        m_resourcePools.destroyTexture(*texture->second);
        m_textures.erase(texture);
    } else {
        LOG_WARN("Attempt to destroy not existing texture - {}, will ignore", name);
    }
}

void TextureManager::destroyAll() {
    LOG_DEBUG("Destroying all textures");
    for (auto& texture : m_textures | std::views::values)
        m_resourcePools.destroyTexture(*texture);
    m_textures.clear();
}

}  // namespace sl