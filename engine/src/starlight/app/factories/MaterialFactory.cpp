#include "MaterialFactory.hh"

#include <span>
#include <boost/algorithm/string.hpp>

#include <kstd/containers/FlatMap.hh>
#include <kstd/Algorithms.hh>

#include "starlight/core/Json.hh"
#include "starlight/core/Globals.hh"
#include "starlight/math/Serialization.hh"

#include "TextureFactory.hh"

namespace sl {

static Material::Properties getDefaultProperties() {
    auto& textureFactory = TextureFactory::get();
    return Material::Properties{
        .diffuseMap   = textureFactory.getDefaultDiffuseMap(),
        .specularMap  = textureFactory.getDefaultSpecularMap(),
        .normalMap    = textureFactory.getDefaultNormalMap(),
        .diffuseColor = MaterialFactory::defaultDiffuseColor,
        .shininess    = MaterialFactory::defaultShininess
    };
}

using MaterialFile = kstd::StaticFlatMap<std::string, std::string, 8u>;

static std::optional<MaterialFile> parseMaterialFile(
  std::span<const std::string> file
) {
    MaterialFile tokens;
    for (const auto& token : file) {
        const auto assignment = token.find_first_of("=");
        if (assignment == token.npos) {
            log::error("Invalid entry in material file: {}", token);
            return {};
        }

        auto k = token.substr(0, assignment);
        auto v = token.substr(assignment + 1);

        boost::algorithm::trim(k);
        boost::algorithm::trim(v);

        log::debug("Parsed material property: '{}' = '{}'", k, v);

        if (tokens.contains(k)) {
            log::error("Property '{}' defined more than once", k);
            return {};
        }

        tokens.insert(k, v);
    }
    return tokens;
}

static std::optional<Material::Properties> loadProperties(const std::string& path) {
    log::info("Loading material properties file: {}", path);

    if (not kstd::isFile(path)) {
        log::error("Could not find file: '{}'", path);
        return {};
    }

    if (auto f = parseMaterialFile(kstd::readLines(path)); not f) {
        log::error("Could not parse material file");
        return {};
    } else {
        auto props = getDefaultProperties();
        auto& tf   = sl::TextureFactory::get();

        if (auto diffuseColor = f->get("DIFFUSE_COLOR"); diffuseColor)
            props.diffuseColor = fromString<Vec4<f32>>(*diffuseColor);

        if (auto diffuseMap = f->get("DIFFUSE_MAP"); diffuseMap)
            props.diffuseMap = tf.loadFlat(*diffuseMap);

        if (auto specularMap = f->get("SPECULAR_MAP"); specularMap)
            props.specularMap = tf.loadFlat(*specularMap);

        if (auto normalMap = f->get("NORMAL_MAP"); normalMap)
            props.normalMap = tf.loadFlat(*normalMap);

        try {
            if (auto shininess = f->get("SHININESS"); shininess)
                props.shininess = std::stof(*shininess);
        } catch (std::invalid_argument& e) {
            log::error("Could not parse property: {}", e.what());
            return {};
        }

        return props;
    }
}

MaterialFactory::MaterialFactory() { createDefault(); }

kstd::SharedPtr<Material> MaterialFactory::load(const std::string& name) {
    log::info("Loading material: {}", name);
    if (auto resource = find(name); resource) return resource;

    const auto& materialsPath = Globals::get().getConfig().paths.materials;
    const auto fullPath       = fmt::format("{}/{}.starmtl", materialsPath, name);

    if (auto props = loadProperties(fullPath); props) {
        return create(name, *props);
    }

    log::warn("Could not load material: '{}'", name);
    return nullptr;
}

kstd::SharedPtr<Material> MaterialFactory::create(
  const std::string& name, const Material::Properties& properties
) {
    return save(kstd::makeShared<Material>(properties, name));
}

kstd::SharedPtr<Material> MaterialFactory::getDefault() { return m_defaultMaterial; }

void MaterialFactory::createDefault() {
    m_defaultMaterial =
      save(kstd::makeShared<Material>(getDefaultProperties(), "Material.Default"));
}

}  // namespace sl