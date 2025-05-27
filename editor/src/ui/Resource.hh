#pragma once

#include <starlight/core/Core.hh>

namespace sle {

struct Resource {
    enum class Type : sl::u16 {
        unknown,
        directory,
        shader,
        texture,
        material,
        model,
        wavefrontObject,
        wavefrontMaterial
    };

    explicit Resource(const std::string& fullPath, const std::string& assetPath);
    bool isDirectory() const;

    std::string fullPath;
    std::string assetPath;
    std::string name;
    std::string extension;
    Type type;
    std::string_view thumbnail;
};
}  // namespace sle
