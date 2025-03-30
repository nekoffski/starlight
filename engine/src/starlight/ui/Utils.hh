#pragma once

#include <memory>

#include "starlight/renderer/camera/Camera.hh"
#include "starlight/core/Utils.hh"
#include "Core.hh"

namespace sl {

// TODO: measure if it causes bottlenecks ->
// if yes implement lightweight wrapper

bool checkbox(const std::string& label, bool& value);

void indent(Callback&& callback);

void pushTextColor(const Vec3<f32>& color);
void popTextColor(int count = 1);
void withColor(const Vec3<f32>& color, Callback&& callback);

void namedScope(const std::string& name, Callback&& callback);

void group(Callback&& callback);

void treeNode(
  const std::string& name, std::function<void()>&& callback,
  i32 flags = ImGuiTreeNodeFlags_None
);

bool wasItemClicked();

template <typename... Args> bool text(const std::string& fmt, Args&&... args) {
    ImGui::Text(
      "%s", fmt::format(fmt::runtime(fmt), std::forward<Args>(args)...).c_str()
    );
    return ImGui::IsItemClicked();
}

bool button(const std::string& text, const Vec2<f32>& size = { 0.0f, 0.0f });

void closeCurrentPopup();
void sameLine();
void lineBreak();
void separator();

float getFrameHeight();

}  // namespace sl
