#pragma once

#include <variant>

namespace sl {

struct RCNoop {};

using RendererCommand = std::variant<RCNoop>;

}  // namespace sl
