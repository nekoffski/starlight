#include "ResourcesView.hh"

// #include <starlight/event/EventProxy.hh>
// #include <starlight/renderer/Material.hh>
#include <starlight/app/factories/MeshFactory.hh>
#include <starlight/app/factories/MaterialFactory.hh>
#include <starlight/app/factories/TextureFactory.hh>
#include <starlight/app/factories/ModelFactory.hh>

#include "Console.hh"

namespace sle {

static constexpr sl::u32 rowSize = 5;

static sl::f32 getThumbnailWidth() {
    return ImGui::GetWindowWidth() / static_cast<sl::f32>(rowSize + 1);
}

// // TODO: add concepts
static void renderResourceTab(
  const std::string& name, auto resources, auto&& create, auto&& render,
  auto&& renderThumbnail
) {
    if (sl::ui::button(fmt::format("Create new {}", name))) {
        // sl::EventProxy::get().emit<events::SetResourceUICallback>(
        //   [&, render, resource = create()]() { render(resource); }
        // );
    }

    sl::ui::separator();
    const auto width = getThumbnailWidth();

    for (sl::u64 i = 0; i < resources.size(); ++i) {
        if (i % rowSize != 0) sl::ui::sameLine();

        auto& resource = *resources[i];

        sl::ui::group([&]() {
            renderThumbnail(resource, width);
            sl::ui::text("{}", resource.name);
        });

        if (sl::ui::wasItemClicked()) {
            editorWriteDebug("{} selected: {}", name, resource.name);
            //         sl::EventProxy::get().emit<events::SetResourceUICallback>(
            //           [render = std::move(render), resource = resources[i]]() {
            //               render(resource);
            //           }
            //         );
        }
    }
}

ResourcesView::ResourcesView(Data& data
) : m_data(data), m_tabMenu("resource-view-tab-menu") {
    m_tabMenu.addTab("Materials", [&]() { renderMaterialsTab(); })
      .addTab("Cubemaps", [&]() { renderCubemapsTab(); })
      .addTab("Textures", [&]() { renderTexturesTab(); })
      .addTab("Models", [&]() { renderModelsTab(); })
      .addTab("Meshes", [&]() { renderMeshesTab(); });
}

void ResourcesView::render() { m_tabMenu.render(); }

void ResourcesView::renderMeshesTab() {
    // for (auto& mesh : sl::MeshFactory::get().getAll()) {
    //     sl::ui::text("{}", mesh.name);
    // }
}

void ResourcesView::renderMaterialsTab() {
    renderResourceTab(
      "Material", sl::MaterialFactory::get().getValues(), [&]() {},
      [&](auto& material) {},
      [&](auto& material, const auto width) {
          showTexture(*material.diffuseMap, width);
      }
    );
}

void ResourcesView::renderCubemapsTab() {}

void ResourcesView::renderModelsTab() {}

void ResourcesView::renderTexturesTab() {
    renderResourceTab(
      "Texture", sl::TextureFactory::get().getValues(sl::Texture::Type::flat),
      [&]() {}, [&](auto& texture) {},
      [&](auto& texture, const auto width) { showTexture(texture, width); }
    );
}

void ResourcesView::showTexture(sl::Texture& texture, sl::f32 width) {
    m_data.getTextureImage(texture)
      ->show({ width, width }, { 0, 0 }, { 1.0f, 1.0f });
}

}  // namespace sle