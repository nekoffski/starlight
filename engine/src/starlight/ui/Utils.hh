#pragma once

#include <memory>

#include "starlight/renderer/camera/Camera.hh"
#include "starlight/core/Utils.hh"
#include "Core.hh"

namespace sl {

void verticalLine(
  float padding = 4.0f, const Vec4<f32> color = { 255, 255, 255, 150 },
  float thickness = 1.0f
);
bool checkbox(const std::string& label, bool& value);

void padding(const Vec2<f32>& padding);

template <typename C>
requires Callable<C>
void conditionallyDisabled(C&& callback, bool disabled) {
    if (disabled) ImGui::BeginDisabled();
    callback();
    if (disabled) ImGui::EndDisabled();
}

template <typename C>
requires Callable<C>
void indent(C&& callback) {
    ImGui::Indent();
    callback();
    ImGui::Unindent();
}

template <typename C>
requires Callable<C>
void namedScope(const std::string& name, C&& callback) {
    ImGui::PushID(name.c_str());
    callback();
    ImGui::PopID();
}

template <typename C>
requires Callable<C>
void group(C&& callback) {
    ImGui::BeginGroup();
    callback();
    ImGui::EndGroup();
}

template <typename C>
requires Callable<C>
void treeNode(const std::string& name, C&& callback, i32 flags) {
    if (ImGui::TreeNodeEx(name.c_str(), flags)) {
        callback();
        ImGui::TreePop();
    }
}

template <typename C>
requires Callable<C>
void child(
  const std::string& name, C&& callback, const Vec2<f32>& size = { 0.0f, 0.0f },
  i32 flags = ImGuiWindowFlags_None
) {
    ImGui::BeginChild(name.c_str(), ImVec2{ size.x, size.y }, flags);
    callback();
    ImGui::EndChild();
}

template <typename C>
requires Callable<C>
void openTreeNode(const std::string& name, C&& callback) {
    treeNode(name, std::forward<C>(callback), ImGuiTreeNodeFlags_DefaultOpen);
}

bool wasItemClicked();

bool button(const std::string& text, const Vec2<f32>& size = { 0.0f, 0.0f });

void closeCurrentPopup();
void sameLine();
void lineBreak();
void separator();

float getFrameHeight();

}  // namespace sl
