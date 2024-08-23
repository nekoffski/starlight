#include "TextureManager.hh"

#include <kc/core/Log.h>

#include <fmt/core.h>

#include "starlight/core/math/Core.hh"
#include "starlight/renderer/gpu/Texture.hh"
#include "starlight/renderer/gpu/ResourcePools.hh"
#include "starlight/resource/configs/ImageData.hh"

namespace sl {

TextureManager::TextureManager(ResourcePools& resourcePools) :
    ResourceManager("Texture"), m_resourcePools(resourcePools),
    m_defaultTexture(nullptr), m_defaultNormalMap(nullptr),
    m_defaultSpecularMap(nullptr) {
    createDefaultTexture();
    createDefaultNormalMap();
    createDefaultSpecularMap();
}

TextureManager::~TextureManager() {
    forEach([&]([[maybe_unused]] u64, Texture* texture) {
        m_resourcePools.destroyTexture(*texture);
    });
}

Texture* TextureManager::load(const std::string& name) {
    LOG_DEBUG("Loading texture '{}'", name);

    if (auto texture = acquire(name); texture) {
        LOG_INFO(
          "Texture {} already loaded, returning pointer to the existing one", name
        );
        return texture;
    }

    auto imageData = ImageData::loadFromFile(name);

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

    auto texture = m_resourcePools.createTexture(props, imageData->pixels);
    return storeResource(props.name, texture->getId(), texture);
}

Texture* TextureManager::loadCubeTexture(const std::string& name) {
    // +X, -X, +Y, -Y, +Z, -Z
    LOG_DEBUG("Loading cube map: {}", name);
    if (auto texture = acquire(name); texture) {
        LOG_INFO(
          "Texture {} already loaded, returning pointer to the existing one", name
        );
        return texture;
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

    std::vector<u8> buffer;

    for (u8 i = 0u; i < cubeFaces; ++i) {
        const auto& path = texturePaths[i];

        const auto imageData =
          ImageData::loadFromFile(path, ImageData::Orientation::nonFlipped);
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
        std::memcpy(&buffer[offset], imageData->pixels.data(), chunkSize);
    }

    auto texture = m_resourcePools.createTexture(props, buffer);
    return storeResource(props.name, texture->getId(), texture);
}

void TextureManager::destroyInternals(Texture* texture) {
    m_resourcePools.destroyTexture(*texture);
}

static void setColor(uint32_t i, const Vec4<f32>& color, std::vector<u8>& pixels) {
    pixels[i]     = color.x;
    pixels[i + 1] = color.y;
    pixels[i + 2] = color.z;
    pixels[i + 3] = color.w;
}

void TextureManager::createDefaultTexture() {
    LOG_TRACE("Creating default texture");
    Texture::Properties props{
        .width         = 256,
        .height        = 256,
        .channels      = 4,
        .isTransparent = false,
        .isWritable    = false,
        .name          = defaultTextureName,
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
            ? Vec4<f32>{ 0.0f, 0.0f, 0.0f, 255.0f }
            : Vec4<f32>{ 255.0f, 255.0f, 255.0f, 255.0f },
          pixels
        );
    }
    auto texture            = m_resourcePools.createTexture(props, pixels);
    Texture::defaultDiffuse = storeResource(props.name, texture->getId(), texture);
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
    auto texture             = m_resourcePools.createTexture(props, pixels);
    Texture::defaultSpecular = storeResource(props.name, texture->getId(), texture);
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

    Vec4<f32> zAxis{ 0.0f, 0.0f, 255.0f, 255.0f };
    for (int i = 0; i < pixels.size(); i += props.channels)
        setColor(i, zAxis, pixels);

    auto texture           = m_resourcePools.createTexture(props, pixels);
    Texture::defaultNormal = storeResource(props.name, texture->getId(), texture);
    LOG_TRACE("Default specular normal map created");
}

}  // namespace sl