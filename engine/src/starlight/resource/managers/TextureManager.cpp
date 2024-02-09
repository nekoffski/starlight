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

    const auto imageData =
      STBImageData::load(STBImageData::Properties{ .name = name, .flip = true });
    ASSERT(imageData, "Could not load image: {}", name);

    sl::Texture::Properties props{
        .width         = imageData->width,
        .height        = imageData->height,
        .channels      = imageData->channels,
        .isTransparent = imageData->isTransparent,
        .isWritable    = false,
        .name          = name,
        .type          = Texture::Type::flat
    };

    m_textures[name] = m_resourcePools.createTexture(
      props,
      std::span<u8>(
        imageData->pixels, imageData->width * imageData->height * imageData->channels
      )
    );
    auto texture                     = m_textures[name];
    m_texturesById[texture->getId()] = texture;

    return texture;
}

Texture* TextureManager::loadCubeTexture(const std::string& name) {
    // +X, -X, +Y, -Y, +Z, -Z

    LOG_DEBUG("Loading cube map: {}", name);
    if (auto texture = m_textures.find(name); texture != m_textures.end()) {
        LOG_INFO(
          "Texture {} already loaded, returning pointer to the existing one", name
        );
        return texture->second;
    }

    // TODO: assuming jpg for now but implement some enum to make it configurable
    static std::string extension  = "jpg";
    static constexpr u8 cubeFaces = 6u;

    std::array<std::string, cubeFaces> texturePaths = {
        fmt::format("{}_r.{}", name, extension),
        fmt::format("{}_l.{}", name, extension),
        fmt::format("{}_u.{}", name, extension),
        fmt::format("{}_d.{}", name, extension),
        fmt::format("{}_f.{}", name, extension),
        fmt::format("{}_b.{}", name, extension)
    };

    sl::Texture::Properties props{
        .width         = 0,
        .height        = 0,
        .channels      = 0,
        .isTransparent = false,
        .isWritable    = false,
        .name          = name,
        .type          = Texture::Type::cubemap
    };

    STBImageData::Properties imageProps;
    imageProps.flip = false;

    std::vector<u8> buffer;

    for (u8 i = 0u; i < cubeFaces; ++i) {
        const auto& path = texturePaths[i];

        imageProps.name      = path;
        const auto imageData = STBImageData::load(imageProps);
        ASSERT(imageData, "Could not load cube map face: {}", path);

        const auto chunkSize =
          imageData->width * imageData->height * imageData->channels;

        if (buffer.empty()) {
            props.width    = imageData->width;
            props.height   = imageData->height;
            props.channels = imageData->channels;

            buffer.resize(chunkSize * cubeFaces, 0u);
        }

        ASSERT(
          imageData->width == props.width && imageData->height == props.height
            && imageData->channels == props.channels,
          "Cube map faces have different size"
        );

        u64 offset = i * chunkSize;
        std::memcpy(&buffer[offset], imageData->pixels, chunkSize);
    }

    m_textures[name]                 = m_resourcePools.createTexture(props, buffer);
    auto texture                     = m_textures[name];
    m_texturesById[texture->getId()] = texture;

    return texture;
}

Texture* TextureManager::acquire(const std::string& name) const {
    if (auto texture = m_textures.find(name); texture != m_textures.end()) {
        return texture->second;
    } else {
        LOG_WARN("Texture {} not found", name);
        return nullptr;
    }
}

Texture* TextureManager::acquire(u64 id) const {
    if (auto texture = m_texturesById.find(id); texture != m_texturesById.end()) {
        return texture->second;
    } else {
        LOG_WARN("Texture with id={} not found", id);
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
        .name          = "DefaultTexture",
        .type          = Texture::Type::flat
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
        .name          = defaultSpecularMapName,
        .type          = Texture::Type::flat
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
        .name          = defaultNormalMapName,
        .type          = Texture::Type::flat
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