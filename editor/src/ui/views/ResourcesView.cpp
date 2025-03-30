#include "ResourcesView.hh"

#include <starlight/app/factories/MeshFactory.hh>
#include <starlight/app/factories/MaterialFactory.hh>
#include <starlight/app/factories/TextureFactory.hh>
#include <starlight/app/factories/ModelFactory.hh>
#include <starlight/core/TaskQueue.hh>

#include "Console.hh"

namespace sle {

static constexpr sl::u32 rowSize = 5;

static sl::f32 getThumbnailWidth() {
    return ImGui::GetWindowWidth() / static_cast<sl::f32>(rowSize + 1);
}

// // TODO: add concepts
static void renderResourceTab(
  const std::string& name, auto resources, auto&& create, auto&& renderThumbnail,
  auto&& setInspectorCallback
) {
    if (sl::button(fmt::format("Create new {}", name))) {
        // sl::EventProxy::get().emit<events::SetResourceUICallback>(
        //   [&, render, resource = create()]() { render(resource); }
        // );
    }

    sl::separator();
    const auto width = getThumbnailWidth();

    for (sl::u64 i = 0u; i < resources.size(); ++i) {
        if (i % rowSize != 0) sl::sameLine();

        auto& resource = *resources[i];

        sl::group([&]() {
            renderThumbnail(resource, width);
            sl::text("{}", resource.name);
        });

        if (sl::wasItemClicked()) {
            editorWriteDebug("{} selected: {}", name, resource.name);
            setInspectorCallback(resource);
        }
    }
}

ResourcesView::ResourcesView(Widget::State& state
) : Widget(state), m_tabMenu("resource-view-tab-menu") {
    m_tabMenu.addTab("Materials", [&]() { renderMaterialsTab(); })
      .addTab("Cubemaps", [&]() { renderCubemapsTab(); })
      .addTab("Textures", [&]() { renderTexturesTab(); })
      .addTab("Models", [&]() { renderModelsTab(); })
      .addTab("Meshes", [&]() { renderMeshesTab(); });
}

void ResourcesView::render() { m_tabMenu.render(); }

void ResourcesView::renderMeshesTab() {
    // for (auto& mesh : sl::MeshFactory::get().getAll()) {
    //     sl::text("{}", mesh.name);
    // }
}

void ResourcesView::renderMaterial(sl::Material& material) {
    sl::namedScope(material.name, [&]() {
        sl::text(ICON_FA_SCROLL "  Material - {}", material.name);
        sl::separator();

        const auto width = ImGui::GetWindowWidth() / 1.1f;
        auto textures = sl::TextureFactory::get().getValues(sl::Texture::Type::flat);
        bool textureChanged = false;

        sl::text("Diffuse color:");
        ImGui::ColorEdit4(
          "##diffuse-color", sl::math::value_ptr(material.diffuseColor)
        );

        sl::separator();
        sl::text("Shininess:");
        ImGui::SliderFloat("##shininess", &material.shininess, 0.0f, 64.0f);

        auto diffuseMap = material.diffuseMap;
        sl::separator();
        sl::immediateCombo(
          "Diffuse map", material.diffuseMap->name, textures,
          [&](auto& texture) {
              diffuseMap     = texture;
              textureChanged = true;
          }
        );
        showImage(*material.diffuseMap, width);

        auto specularMap = material.specularMap;
        sl::separator();
        sl::immediateCombo(
          "Specular map", material.specularMap->name, textures,
          [&](auto& texture) {
              specularMap    = texture;
              textureChanged = true;
          }
        );
        showImage(*material.specularMap, width);

        auto normalMap = material.normalMap;
        sl::separator();
        sl::immediateCombo(
          "Normal map", material.normalMap->name, textures,
          [&](auto& texture) {
              normalMap      = texture;
              textureChanged = true;
          }
        );
        showImage(*material.normalMap, width);

        if (textureChanged) {
            sl::TaskQueue::get().callPostFrame(
              [&material, diffuseMap, specularMap, normalMap]() mutable {
                  material.diffuseMap  = diffuseMap;
                  material.specularMap = specularMap;
                  material.normalMap   = normalMap;
              }
            );
        }
    });
}

void ResourcesView::renderMaterialsTab() {
    auto onCreate = []() {};

    auto onThumbnail = [&](auto& material, const auto width) {
        showImage(*material.diffuseMap, width);
    };

    auto onClicked = [&](auto& material) {
        setInspectorCallback([&] { renderMaterial(material); });
    };

    renderResourceTab(
      "Material", sl::MaterialFactory::get().getValues(), onCreate, onThumbnail,
      onClicked
    );
}

void ResourcesView::renderCubemapsTab() {}

void ResourcesView::renderModelsTab() {}

void ResourcesView::renderTexturesTab() {
    auto onCreate = []() {};

    auto onThumbnail = [&](auto& texture, const auto width) {
        showImage(texture, width);
    };

    auto onClicked = [&](auto& texture) {};

    renderResourceTab(
      "Texture", sl::TextureFactory::get().getValues(sl::Texture::Type::flat),
      onCreate, onThumbnail, onClicked
    );
}

}  // namespace sle