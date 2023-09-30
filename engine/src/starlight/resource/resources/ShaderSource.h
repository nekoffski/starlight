#pragma once

#include <string>
#include <optional>

#include "starlight/core/utils/FileSystem.h"

namespace sl {

class ShaderSource {
public:
    explicit ShaderSource(
      std::string_view fullPath, std::string_view name, std::string_view source
    );

    static std::optional<ShaderSource> create(
      std::string_view name, const FileSystem& fs = fileSystem
    );

private:
    std::string m_source;

public:
    const char* data;
    std::size_t size;
};

}  // namespace sl