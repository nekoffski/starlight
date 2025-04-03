#pragma once

#include "starlight/ui/Core.hh"

namespace sl {

void pushTextColor(const Vec3<f32>& color);
void popTextColor(int count = 1);

template <typename F>
requires Callable<F>
void withColor(const Vec3<f32>& color, F&& callback) {
    pushTextColor(color);
    callback();
    popTextColor();
}

template <typename... Args> bool text(const std::string& fmt, Args&&... args) {
    ImGui::Text(
      "%s", fmt::format(fmt::runtime(fmt), std::forward<Args>(args)...).c_str()
    );
    return ImGui::IsItemClicked();
}

}  // namespace sl
