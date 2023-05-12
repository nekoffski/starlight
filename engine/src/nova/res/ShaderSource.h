#pragma once

#include <string>
#include <optional>

#include "Resource.h"
#include "nova/core/FileSystem.h"

namespace nova::res {

class ShaderSource : public Resource {
   public:
    explicit ShaderSource(
        std::string_view fullPath, std::string_view name, std::string_view source
    );

    static std::optional<ShaderSource> create(
        std::string_view name, const core::FileSystem& fs = core::fileSystem
    );

   private:
    std::string m_source;

   public:
    const char* data;
    std::size_t size;
};

}  // namespace nova::res