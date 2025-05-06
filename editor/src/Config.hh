#pragma once

#include <optional>

#include <starlight/core/math/Vec.hh>
#include <starlight/core/Config.hh>

namespace sle {

struct Config {
    static std::optional<Config> fromJson(
      const std::string& path,
      const kstd::FileSystem& fs = kstd::GlobalFileSystem::get()
    );
    sl::Config toEngineConfig() const;

    std::string assetsRoot;

    sl::Vec2<sl::u32> windowSize;
    sl::Vec2<sl::f32> layoutSizeRatio;

    std::optional<std::string> initialScene;
};

}  // namespace sle
