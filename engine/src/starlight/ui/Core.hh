#pragma once

#include <string>
#include <functional>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <fmt/core.h>

#include "starlight/core/Core.hh"
#include "starlight/math/Core.hh"

namespace sl {

constexpr Vec2<f32> parentWidth = { -1.0f, 0.0f };

using Callback = std::function<void()>;

}  // namespace sl
