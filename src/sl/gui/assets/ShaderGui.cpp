#include "ShaderGui.h"

#include <imgui/imgui.h>
#include <imgui_sugar.hpp>

#include "sl/async/AsyncManager.hpp"
#include "sl/gfx/Shader.h"
#include "sl/gfx/ShaderManager.h"
#include "sl/gui/Utils.hpp"

namespace sl::gui::assets {

ShaderGui::Provider::Provider(std::shared_ptr<gfx::Shader> shader, ShaderGui::Params& params)
    : m_shader(shader)
    , m_params(params) {
}

void ShaderGui::Provider::render() {
    if (auto shader = m_shader.lock(); shader) {
        ImGui::Text("Shader resource");
        ImGui::Separator();

        with_OpenedTreeNode("Paths:") {
            ImGui::Text("%s", shader->getVertexShaderPath().c_str());
            ImGui::Text("%s", shader->getFragmentShaderPath().c_str());
            ImGui::Text("%s", shader->getGeometryShaderPath().c_str());
        }

        ImGui::Separator();

        with_OpenedTreeNode("Build") {
            if (ImGui::Button("Recompile", ImVec2(ImGui::GetWindowWidth() * 0.85f, 0.0f))) {
                shader->disable();

                try {
                    gfx::ShaderManager::get()->recompileShader(*shader);
                } catch (core::ShaderError& err) {
                    LOG_WARN("Could not recompile shader due to {}", err.getDetails());

                    using namespace event;
                    EventManager::get()->emit<DisplayErrorEvent>(err.asString()).toAll();
                }
            }

            ImGui::Text("\n");

            if (ImGui::Checkbox("Recompile on save", &m_params.recompileOnSave))
                processRecompileOnSaveRequest(shader);
        }
    }
}

void ShaderGui::Provider::processRecompileOnSaveRequest(std::shared_ptr<gfx::Shader>& shader) {
    auto& taskHandle = m_params.taskHandle;

    if (m_params.recompileOnSave) {
        taskHandle = async::AsyncManager::get()->addPeriodicTask<gfx::RecompileShaderOnUpdate>(shader);
    } else {
        if (taskHandle.has_value()) {
            taskHandle.value().disable();
            taskHandle.reset();
        }
    }
}

std::unique_ptr<AssetGuiProvider> ShaderGui::createGuiProvider(std::shared_ptr<gfx::Shader> shader) {
    return std::make_unique<Provider>(shader, m_params[shader->getId()]);
}

}