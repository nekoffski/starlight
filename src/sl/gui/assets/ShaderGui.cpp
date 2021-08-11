#include "ShaderGui.h"

#include "sl/async/AsyncManager.hpp"
#include "sl/gfx/Shader.h"
#include "sl/gfx/ShaderManager.h"

namespace sl::gui::assets {

ShaderGui::Provider::Provider(std::shared_ptr<gfx::Shader> shader, ShaderGui::Params& params)
    : m_shader(shader)
    , m_params(params) {
}

void ShaderGui::Provider::render(GuiApi& gui) {
    if (auto shader = m_shader.lock(); shader) {
        gui.displayText("Shader resource");
        gui.breakLine();

        if (gui.beginTreeNode("Paths:")) {
            gui.displayText(shader->getVertexShaderPath());
            gui.displayText(shader->getFragmentShaderPath());
            gui.displayText(shader->getGeometryShaderPath());
            gui.popTreeNode();
        }

        gui.breakLine();

        if (gui.beginTreeNode("Build")) {
            if (gui.button("Recompile", gui.getCurrentWindowWidth() * 0.85f)) {
                shader->disable();

                try {
                    gfx::ShaderManager::get()->recompileShader(*shader);
                } catch (core::ShaderError& err) {
                    SL_WARN("Could not recompile shader due to {}", err.getDetails());

                    using namespace event;
                    EventManager::get()->emit<DisplayErrorEvent>(err.as<std::string>()).toAll();
                }
            }

            gui.displayText("\n");

            if (gui.checkbox("Recompile on save", m_params.recompileOnSave))
                processRecompileOnSaveRequest(shader);

            gui.popTreeNode();
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