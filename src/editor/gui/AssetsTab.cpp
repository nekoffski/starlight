#include "AssetsTab.h"

#include <any>
#include <imgui_sugar.hpp>
#include <ranges>

#include "editor/DebugConsole.hpp"
#include "sl/core/Errors.hpp"
#include "sl/core/InputManager.h"
#include "sl/geom/GeometryManager.h"
#include "sl/gfx/ShaderManager.h"
#include "sl/gfx/TextureManager.h"
#include "sl/glob/Globals.h"
#include "sl/gui/Utils.h"

namespace editor::gui {

constexpr int padding = 65;

static void validateAssetName(const std::string& name) {
    using namespace sl::core;

    if (name.empty()) throw AssetError("Asset name cannot be empty");
}

AssetsTab::AssetsTab(std::shared_ptr<SharedState> sharedState)
    : Widget(sharedState), m_fileBrowser("Assets file browser") {}

void AssetsTab::render() {
    with_Group {
        if (ImGui::Button(ICON_FA_PLUS "  Load new asset")) {
            resetArgs();
            ImGui::OpenPopup("AssetLoadPopUp");
        }

        with_Popup("AssetLoadPopUp") showLoaderPopUp();

        ImGui::Text("\n");

        static std::string selectedAssetName = "";

        auto displayAssetSection = [&](const std::string& header, auto& assetsMap) {
            with_Group {
                with_OpenedTreeNode(header.c_str()) {
                    for (auto& [name, asset] : assetsMap) {
                        bool isSelected = (name == selectedAssetName);

                        if (isSelected) sl::gui::pushTextColor(sl::gui::selectedEntryColor);

                        ImGui::Text("%s", name.c_str());

                        if (isSelected) sl::gui::popTextColor();

                        if (ImGui::IsItemClicked()) {
                            selectedAssetName = name;
                            m_sharedState->activeAssetGuiProvider =
                                m_assetsGui.createGuiProvider(asset);
                        }
                    }
                }
            }
        };

        auto& cubemaps = m_sharedState->assetManager.getCubemaps().getAll();
        displayAssetSection("Cubemaps", cubemaps);
        ImGui::SameLine();

        auto& meshes = m_sharedState->assetManager.getMeshes().getAll();
        displayAssetSection("Meshes", meshes);
        ImGui::SameLine();

        auto shaders = m_sharedState->assetManager.getShaders().getAll();
        auto& globalShaders = sl::glob::Globals::get().shaders->shadersByName;

        for (auto& [name, shader] : globalShaders) shaders[name] = shader;

        displayAssetSection("Shader", shaders);

        ImGui::SameLine();

        auto& textures = m_sharedState->assetManager.getTextures().getAll();
        displayAssetSection("Textures", textures);
    }
}

void AssetsTab::resetArgs() {
    m_assetsArgs.assetName = "";
    m_assetsArgs.activeItem = 0;
    m_assetsArgs.modelName = "/tow/tower.obj";
    m_assetsArgs.faces = {"/skybox/right.jpg",  "/skybox/left.jpg",  "/skybox/top.jpg",
                          "/skybox/bottom.jpg", "/skybox/front.jpg", "/skybox/back.jpg"};
}

void AssetsTab::showLoaderPopUp() {
    static const std::vector<std::string> labels = {"Cubemap", "Texture", "Model", "Shader"};

    sl::gui::combo("##AssetsCombo", &m_assetsArgs.activeItem, labels);

    m_loadClicked = false;

    if (ImGui::Button("Quit")) ImGui::CloseCurrentPopup();

    ImGui::SameLine();
    if (ImGui::Button("Load")) {
        m_loadClicked = true;
    }

    ImGui::SameLine(250);

    with_Group {
        ImGui::Text("Name");
        ImGui::SameLine(padding);
        sl::gui::textInput("##Name", m_assetsArgs.assetName);

        try {
            switch (m_assetsArgs.activeItem) {
                case 0: {
                    handleCubemapLoader();
                    break;
                }

                case 1: {
                    handleTextureLoader();
                    break;
                }

                case 2: {
                    handleModelLoader();
                    break;
                }

                case 3: {
                    handleShaderLoader();
                    break;
                }
            }
        } catch (sl::core::AssetError& err) {
            m_errorDialog.setErrorMessage(err.getDetails());
            WRITE_DEBUG("%s", err.asString());
            m_loadClicked = false;
        }

        if (m_loadClicked) ImGui::CloseCurrentPopup();
    }

    m_errorDialog.show();
}

void AssetsTab::handleShaderLoader() {
    sl::gui::labeledTextInput("Vertex shader", m_assetsArgs.faces[0], padding);
    if (ImGui::IsItemClicked())
        m_fileBrowser.open([&v = m_assetsArgs.faces[0]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput("Fragment shader", m_assetsArgs.faces[1], padding);
    if (ImGui::IsItemClicked())
        m_fileBrowser.open([&v = m_assetsArgs.faces[1]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput("Geometry shader", m_assetsArgs.faces[2], padding);
    if (ImGui::IsItemClicked())
        m_fileBrowser.open([&v = m_assetsArgs.faces[2]](const std::string& value) { v = value; });

    m_fileBrowser.show();

    if (m_loadClicked) {
        validateAssetName(m_assetsArgs.assetName);

        auto shader = sl::gfx::ShaderManager::get().load(
            m_assetsArgs.faces[0], m_assetsArgs.faces[1], m_assetsArgs.faces[2]);
        // auto shaderAsset = std::make_shared<sl::asset::ShaderAsset>(shader,
        // m_assetsArgs.assetName); m_sharedState->assetManager.addAsset(shaderAsset);
    }
}

void AssetsTab::handleCubemapLoader() {
    sl::gui::labeledTextInput("Right", m_assetsArgs.faces[0], padding);
    if (ImGui::IsItemClicked())
        m_fileBrowser.open([&v = m_assetsArgs.faces[0]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput("Left", m_assetsArgs.faces[1], padding);
    if (ImGui::IsItemClicked())
        m_fileBrowser.open([&v = m_assetsArgs.faces[1]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput("Top", m_assetsArgs.faces[2], padding);
    if (ImGui::IsItemClicked())
        m_fileBrowser.open([&v = m_assetsArgs.faces[2]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput("Bottom", m_assetsArgs.faces[3], padding);
    if (ImGui::IsItemClicked())
        m_fileBrowser.open([&v = m_assetsArgs.faces[3]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput("Front", m_assetsArgs.faces[4], padding);
    if (ImGui::IsItemClicked())
        m_fileBrowser.open([&v = m_assetsArgs.faces[4]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput("Back", m_assetsArgs.faces[5], padding);
    if (ImGui::IsItemClicked())
        m_fileBrowser.open([&v = m_assetsArgs.faces[5]](const std::string& value) { v = value; });

    m_fileBrowser.show();

    if (m_loadClicked) {
        validateAssetName(m_assetsArgs.assetName);

        auto facesPaths = m_assetsArgs.faces;
        const auto& cubemapsPath = sl::glob::Globals::get().config.paths.cubemaps;

        std::ranges::transform(facesPaths, facesPaths.begin(),
                               [&cubemapsPath](const auto& facePath) -> std::string {
                                   return cubemapsPath + facePath;
                               });

        using namespace sl::gfx;

        TextureManager::get()
            .createCubemap()
            .fromPaths(facesPaths)
            .withName(m_assetsArgs.assetName)
            .getAsync([&](auto&& cubemap) { m_sharedState->assetManager.add(std::move(cubemap)); });
    }
}

void AssetsTab::handleModelLoader() {
    sl::gui::labeledTextInput("Model", m_assetsArgs.modelName, padding);
    if (ImGui::IsItemClicked())
        m_fileBrowser.open([&v = m_assetsArgs.modelName](const std::string& value) { v = value; });

    m_fileBrowser.show();

    if (m_loadClicked) {
        validateAssetName(m_assetsArgs.assetName);

        auto model = sl::geom::GeometryManager::get().loadModel(m_assetsArgs.modelName);
        m_sharedState->assetManager.add(model->meshes);
    }
}

void AssetsTab::handleTextureLoader() {
    sl::gui::labeledTextInput("Texture", m_assetsArgs.modelName, padding);

    if (ImGui::IsItemClicked())
        m_fileBrowser.open([&v = m_assetsArgs.modelName](const std::string& value) { v = value; });

    m_fileBrowser.show();

    if (m_loadClicked) {
        validateAssetName(m_assetsArgs.assetName);

        // auto output = std::make_unique<sl::asset::AssetManager::Output<
        // sl::gfx::Texture>>(m_sharedState->assetManager);

        // sl::gfx::Texture::loadAsync(m_assetsArgs.modelName, m_assetsArgs.assetName,
        // std::move(output));

        // sl::gfx::Texture::loadAsync(m_assetsArgs.modelName, m_assetsArgs.assetName, [&](auto&&
        // texture) {
        //     m_sharedState->assetManager.add(std::move(texture));
        // });
    }
}
}  // namespace editor::gui
