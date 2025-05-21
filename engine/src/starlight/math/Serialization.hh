#pragma once

#include "starlight/core/Core.hh"
#include "Core.hh"

namespace sl {

template <> Vec4<f32> fromString<Vec4<f32>>(std::string_view v);

}  // namespace sl
