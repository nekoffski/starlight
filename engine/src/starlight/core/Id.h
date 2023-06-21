#pragma once

#include <cstdint>
#include <numeric>

namespace sl {

using Id               = uint32_t;
constexpr Id invalidId = std::numeric_limits<Id>::max();

}  // namespace sl
