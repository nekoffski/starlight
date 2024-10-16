#pragma once

#include <string>

#include "starlight/core/Core.hh"

namespace sl {

enum class CullMode : u8 { none, front, back, frontAndBack };

std::string cullModeToString(CullMode);
CullMode cullModeFromString(const std::string&);

}  // namespace sl
