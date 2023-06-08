#include "Utils.h"

namespace sl::res {

std::string_view removeExtension(std::string_view path) {
    if (const auto extension = path.find("."); extension != path.npos)
        return std::string_view(path.begin(), extension);
    return path;
}

}  // namespace sl::res
