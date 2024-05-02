#pragma once

#include <memory>

#include <imgui.h>
#include <fmt/core.h>

#include "starlight/renderer/Viewport.h"
#include "starlight/renderer/camera/Camera.h"

#include "starlight/core/math/Core.h"
#include "starlight/renderer/gpu/UIRenderer.h"

namespace sl::ui {

using Callback = std::function<void()>;  // TODO: measure if it causes bottlenecks ->
                                         // if yes implement lightweight wrapper

void checkbox(const std::string& label, bool& value);

void pushFont(Font*);
void popFont();

void pushTextColor(const Vec3f& color);
void popTextColor(int count = 1);
void withColor(const Vec3f& color, Callback&& callback);

void namedScope(const std::string& name, Callback&& callback);

void treeNode(const std::string& name, std::function<void(bool)>&& callback);

template <typename... Args>
bool text(const std::string& formatString, Args&&... args) {
    ImGui::Text(
      "%s", fmt::format(formatString, std::forward<Args>(args)...).c_str()
    );
    return ImGui::IsItemClicked();
}

bool slider(const std::string& name, Vec3f& data, const StepInterval<float>& range);

bool button(const std::string& text, const Vec2f& size = { 0.0f, 0.0f });

void closeCurrentPopup();
void sameLine();
void lineBreak();
void separator();

float getFrameHeight();

}  // namespace sl::ui
