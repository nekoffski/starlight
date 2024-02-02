#pragma once

#include <memory>

#include <imgui.h>
#include <fmt/core.h>

#include "starlight/renderer/gpu/UIRenderer.h"

namespace sl::ui {

using Callback = std::function<void()>;  // TODO: measure if it causes bottlenecks ->
                                         // if yes implement lightweight wrapper

void pushFont(Font*);
void popFont();

void namedScope(const std::string& name, Callback&& callback);

template <typename... Args>
void text(const std::string& formatString, Args&&... args) {
    ImGui::Text(
      "%s", fmt::format(formatString, std::forward<Args>(args)...).c_str()
    );
}

void separator();

float getFrameHeight();

}  // namespace sl::ui