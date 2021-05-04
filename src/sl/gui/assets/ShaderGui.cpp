#include "ShaderGui.h"

namespace sl::gui::assets {

namespace details {
    class RecompileShaderOnUpdate : public task::Task {
    public:
        explicit RecompileShaderOnUpdate(std::shared_ptr<gfx::Shader> shader,
            std::unique_ptr<core::FileSystem> fileSystem = std::make_unique<core::FileSystem>())
            : m_shader(shader)
            , m_fileSystem(std::move(fileSystem)) {
        }

        bool shouldInvoke() override {
            if (auto shader = m_shader.lock(); shader) {
                auto vertexShader = shader->getVertexShaderPath();
                auto fragmentShader = shader->getFragmentShaderPath();

                static auto previousVertexWrite = m_fileSystem->lastWriteTime(vertexShader);
                static auto previousFragmentWrite = m_fileSystem->lastWriteTime(fragmentShader);

                auto newVertexWrite = m_fileSystem->lastWriteTime(vertexShader);
                auto newFragmentWrite = m_fileSystem->lastWriteTime(fragmentShader);

                auto shouldRecompile = previousVertexWrite != newVertexWrite ||
                    previousFragmentWrite != newFragmentWrite;

                previousVertexWrite = newVertexWrite;
                previousFragmentWrite = newFragmentWrite;

                return shouldRecompile;
            }
            return false;
        }

        void invoke() override {
            if (auto shader = m_shader.lock(); shader)
                gfx::ShaderCompiler::compile(*shader);
        }

        std::string getName() const {
            return "RecompileShaderOnUpdate";
        }

    private:
        std::weak_ptr<gfx::Shader> m_shader;
        std::unique_ptr<core::FileSystem> m_fileSystem;
    };

}

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
                gfx::ShaderCompiler::compile(*shader);
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
        taskHandle = TASK_MANAGER().addPeriodicTask<details::RecompileShaderOnUpdate>(shader);
    } else {
        if (taskHandle.has_value()) {
            taskHandle.value().disable();
            taskHandle.reset();
        }
    }
}

std::unique_ptr<AssetGuiProvider> ShaderGui::createGuiProvider(std::shared_ptr<gfx::Shader> shader) {
    return std::make_unique<Provider>(shader, m_params[shader->id]);
}

}