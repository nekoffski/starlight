#include "Resource.h"

#ifndef SL_ASSETS_PATH
#define SL_ASSETS_PATH ""
#endif

namespace sl {

std::string Resource::s_assetsBasePath = SL_ASSETS_PATH;

Resource::Resource(std::string_view fullPath, std::string_view name)
    : fullPath(fullPath), name(name) {}

void Resource::setAssetsBasePath(std::string_view path) { s_assetsBasePath = path; }

std::string_view Resource::getAssetsBasePath() { return s_assetsBasePath; }

}  // namespace sl
