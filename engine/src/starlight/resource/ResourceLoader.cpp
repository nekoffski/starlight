#include "ResourceLoader.h"

#include <fmt/core.h>
#include <kc/json/Json.h>
#include <kc/json/Utils.hpp>

#include <stb.h>

#include "starlight/core/utils/Log.h"

namespace sl {

auto fieldFrom(auto& root) { return kc::json::fieldFrom<kc::json::JsonError>(root); }

template <typename T> T getField(auto& root, const std::string& name) {
    return fieldFrom(root).withName(name).template ofType<T>().get();
}

auto getArray(auto& root, const std::string& name) {
    return fieldFrom(root).withName(name).asArray().get();
}

ResourceLoader::ResourceLoader(const std::string& baseResourcePath, FileSystem& fs) :
    m_baseResourcePath(baseResourcePath), m_fs(fs) {}

std::optional<MaterialConfig> ResourceLoader::loadMaterialConfig(
  const std::string& name
) const {
    const auto fullPath =
      fmt::format("{}/materials/{}.json", m_baseResourcePath, name);

    LOG_TRACE("Loading material config file: {}", fullPath);

    if (not m_fs.isFile(fullPath)) {
        LOG_ERROR("Could not find file: '{}'", fullPath);
        return {};
    }

    try {
        auto root = kc::json::loadJson(m_fs.readFile(fullPath));
        MaterialConfig config;

        config.diffuseColor = getField<Vec4f>(root, "diffuse-color");
        config.diffuseMap   = getField<std::string>(root, "diffuse-map");
        config.specularMap  = getField<std::string>(root, "specular-map");
        config.normalMap    = getField<std::string>(root, "normal-map");
        config.shaderName   = getField<std::string>(root, "shader-name");
        config.shininess    = getField<float>(root, "shininess");

        return config;
    } catch (kc::json::JsonError& e) {
        LOG_ERROR("Could not parse material '{}' file: {}", name, e.asString());
    }
    return {};
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

std::optional<ShaderConfig> ResourceLoader::loadShaderConfig(const std::string& name
) const {
    const auto fullPath =
      fmt::format("{}/shaders/{}.json", m_baseResourcePath, name);

    LOG_TRACE("Loading shader config file: {}", fullPath);

    if (not m_fs.isFile(fullPath)) {
        LOG_ERROR("Could not find file: '{}'", fullPath);
        return {};
    }

    static auto processStages =
      [](const auto& root) -> std::vector<ShaderStageConfig> {
        std::vector<ShaderStageConfig> stages;
        stages.reserve(root.size());

        for (auto& stage : root) {
            const auto file      = getField<std::string>(stage, "file");
            const auto stageName = getField<std::string>(stage, "stage");

            stages.emplace_back(ShaderStage::typeFromString(stageName), file);
        }
        return stages;
    };

    static auto processAttributes =
      [](const auto& root) -> std::vector<ShaderAttributeConfig> {
        std::vector<ShaderAttributeConfig> attributes;
        attributes.reserve(root.size());

        for (auto& attribute : root) {
            const auto type = ShaderAttribute::typeFromString(
              getField<std::string>(attribute, "type")
            );
            const auto size = ShaderAttribute::getTypeSize(type);
            const auto name = getField<std::string>(attribute, "name");

            attributes.emplace_back(name, type, size);
        }

        return attributes;
    };

    static auto processUniforms =
      [](const auto& root) -> std::vector<ShaderUniformConfig> {
        std::vector<ShaderUniformConfig> uniforms;
        uniforms.reserve(root.size());

        for (auto& uniform : root) {
            const auto type =
              ShaderUniform::typeFromString(getField<std::string>(uniform, "type"));
            const auto size  = ShaderUniform::getTypeSize(type);
            const auto name  = getField<std::string>(uniform, "name");
            const auto scope = getField<std::string>(uniform, "scope");

            uniforms.emplace_back(name, size, 0, type, shaderScopeFromString(scope));
        }

        return uniforms;
    };

    try {
        auto root = kc::json::loadJson(m_fs.readFile(fullPath));
        ShaderConfig config;

        config.name           = getField<std::string>(root, "name");
        config.renderpassName = getField<std::string>(root, "renderpass");
        config.stages         = processStages(getArray(root, "stages"));
        config.attributes     = processAttributes(getArray(root, "attributes"));
        config.uniforms       = processUniforms(getArray(root, "uniforms"));
        config.useInstances   = getField<bool>(root, "use-instances");
        config.useLocal       = getField<bool>(root, "use-local");

        return config;
    } catch (kc::json::JsonError& e) {
        LOG_ERROR("Could not parse shader '{}' file: {}", name, e.asString());
    }
    return {};
}

std::optional<std::string> ResourceLoader::loadShaderSource(const std::string& name
) const {
    const auto fullPath = fmt::format("{}/shaders/{}", m_baseResourcePath, name);

    if (not m_fs.isFile(fullPath)) {
        LOG_WARN("Could not find shader file '{}'", fullPath);
        return {};
    }
    return m_fs.readFile(fullPath);
}

}  // namespace sl
