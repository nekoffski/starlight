#pragma once

#include "starlight/core/Core.h"

namespace sl {

enum class PolygonMode : u8 { fill, line, point };

std::string polygonModeToString(PolygonMode polygonMode);
PolygonMode polygonModeFromString(const std::string& polygonName);

}  // namespace sl
