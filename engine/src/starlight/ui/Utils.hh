#pragma once

#include <memory>

#include "starlight/renderer/camera/Camera.hh"
#include "starlight/core/Utils.hh"
#include "Core.hh"

namespace sl {

bool checkbox(const std::string& label, bool& value);

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
