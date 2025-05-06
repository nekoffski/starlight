#include "Config.hh"

#include <starlight/core/Log.hh>
#include <starlight/core/Json.hh>

namespace sle {

void from_json(const nlohmann::json& j, Config& out) {
    const auto& window = j.at("window");
    window.at("width").get_to(out.windowSize.w);
    window.at("height").get_to(out.windowSize.h);

    const auto& layout = j.at("layout");
    layout.at("width").get_to(out.layoutSizeRatio.x);
    layout.at("height").get_to(out.layoutSizeRatio.y);

    j.at("assets-root").get_to(out.assetsRoot);

    if (j.contains("initial-scene"))
        out.initialScene = j.at("initial-scene").get<std::string>();
}

std::optional<Config> Config::fromJson(
  const std::string& path, const kstd::FileSystem& fs
) {
    if (not fs.isFile(path)) {
        sl::log::error("Config file '{}' does not exist", path);
        return {};
    }
    try {
        return nlohmann::json::parse(fs.readFile(path)).get<Config>();
    } catch (const nlohmann::json::parse_error& e) {
        sl::log::error("Could not parse config file '{}' - {}", path, e.what());
    }
    return {};
}

sl::Config Config::toEngineConfig() const {
    return sl::Config{
        .window  = { windowSize.w, windowSize.h, "Starligt Editor" },
        .version = { 1,            0,            0                 },
        .paths  = {
            .textures = fmt::format("{}/textures", assetsRoot),
            .shaders = fmt::format("{}/shaders", assetsRoot),
            .materials = fmt::format("{}/materials", assetsRoot),
            .fonts = fmt::format("{}/fonts", assetsRoot),
        }
    };
}

}  // namespace sle
