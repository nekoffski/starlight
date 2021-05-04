#pragma once

#include <unordered_map>

#include "AssetGuiProvider.h"
#include "sl/core/FileSystem.h"
#include "sl/gfx/Shader.h"
#include "sl/task/TaskManager.h"
#include "sl/utils/Globals.h"

namespace sl::gui::assets {

class ShaderGui {
    struct Params {
        bool recompileOnSave = false;
        std::optional<task::Task::Handle> taskHandle = std::nullopt;
    };

    class Provider : public AssetGuiProvider {
    public:
        explicit Provider(std::shared_ptr<gfx::Shader> shader, Params& params);

        void render(GuiApi& gui);

    private:
        void processRecompileOnSaveRequest(std::shared_ptr<gfx::Shader>& shader);

        std::weak_ptr<gfx::Shader> m_shader;
        Params& m_params;
    };

public:
    std::unique_ptr<AssetGuiProvider> createGuiProvider(std::shared_ptr<gfx::Shader> shader);

private:
    std::unordered_map<int, Params> m_params;
};

}