#include "ResourceLoader.h"

#include <fmt/core.h>
#include <kc/json/Json.h>
#include <kc/json/Utils.hpp>

#include <stb.h>

#include "starlight/core/utils/Log.h"

namespace sl {

auto fieldFrom(auto& root) { return kc::json::fieldFrom<kc::json::JsonError>(root); }

ResourceLoader::ResourceLoader(const std::string& baseResourcePath, FileSystem& fs) :
    m_baseResourcePath(baseResourcePath), m_fs(fs) {}

std::optional<MaterialConfig> ResourceLoader::loadMaterialConfig(
  const std::string& name
) const {
    const auto fullPath =
      fmt::format("{}/materials/{}.nvmat", m_baseResourcePath, name);

    if (not m_fs.isFile(fullPath)) {
        LOG_ERROR("Could not find file: '{}'", fullPath);
        return {};
    }

    MaterialConfig config;

    const auto content = m_fs.readFile(fullPath);

    try {
        auto root = kc::json::loadJson(content);

        config.diffuseColor =
          fieldFrom(root).withName("diffuse-color").ofType<Vec4f>().get();
        config.diffuseMap =
          fieldFrom(root).withName("diffuse-map").ofType<std::string>().get();

        const auto materialType = materialTypeFromString(
          fieldFrom(root).withName("type").ofType<std::string>().get()
        );

        ASSERT(materialType, "Material config must contain material type");
        config.type = materialType.value();

        return config;
    } catch (kc::json::JsonError& e) {
        LOG_ERROR("Could not parse material '{}' file: {}", name, e.what());
        return {};
    }
}

std::optional<STBImageData> ResourceLoader::loadImageData(const std::string& name
) const {
    static constexpr int requiredChannels = 4;
    const auto fullPath = fmt::format("{}/textures/{}", m_baseResourcePath, name);

    LOG_TRACE("Loading image: '{}'", fullPath);

    int width;
    int height;
    int channels;

    const auto pixelsHandle =
      stbi_load(fullPath.c_str(), &width, &height, &channels, requiredChannels);

    if (not pixelsHandle) {
        if (const auto error = stbi_failure_reason(); error)
            LOG_ERROR("Could not load '{}' - '{}'", fullPath, error);
        return {};
    }

    if (channels != requiredChannels) {
        LOG_WARN(
          "Image '{}' has different channels count than required - {} != {}", name,
          requiredChannels, channels
        );
    }

    // todo: implement real check
    bool isTransparent = true;

    STBImageData image(
      pixelsHandle, static_cast<uint32_t>(width), static_cast<uint32_t>(height),
      static_cast<uint8_t>(requiredChannels), isTransparent
    );

    LOG_TRACE(
      "Image loaded: width={}, height={}, channels={}", image.width, image.height,
      image.channels
    );

    return image;
}

}  // namespace sl