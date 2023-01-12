#pragma once

#include <cstdint>
#include <numeric>

namespace nova::core {

using Id               = uint32_t;
constexpr Id invalidId = std::numeric_limits<Id>::max();

}  // namespace nova::core
