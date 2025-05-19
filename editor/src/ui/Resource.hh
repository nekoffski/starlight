#pragma once

#include <starlight/core/Core.hh>
#include <kstd/FileSystem.hh>

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

    explicit Resource(const std::string& fullPath, const kstd::FileSystem& fs);
    bool isDirectory() const;

    std::string fullPath;
    std::string name;
    std::string extension;
    Type type;
    std::string_view thumbnail;
};
}  // namespace sle
