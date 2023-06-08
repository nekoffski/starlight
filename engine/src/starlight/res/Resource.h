#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace sl::res {

class Resource {
   public:
    explicit Resource(std::string_view fullPath, std::string_view name);

    std::string fullPath;
    std::string name;

    static void setAssetsBasePath(std::string_view path);

   protected:
    static std::string_view getAssetsBasePath();

   private:
    static std::string s_assetsBasePath;
};

}  // namespace sl::res