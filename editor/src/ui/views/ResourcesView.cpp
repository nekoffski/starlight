#include "ResourcesView.hh"

#include <kstd/String.hh>

#include <starlight/app/factories/MeshFactory.hh>
#include <starlight/app/factories/MaterialFactory.hh>
#include <starlight/app/factories/TextureFactory.hh>
#include <starlight/core/TaskQueue.hh>

#include "Console.hh"

namespace sle {

ResourcesView::ResourcesView(Widget::State& state)
    : Widget(state)
    , m_folderTexture(sl::TextureFactory::get().loadFlat("folder.png"))
    , m_root(getConfig().assetsRoot, "") {
    build();
}

void ResourcesView::render() {
    sl::text("Project path: {}", m_root.fullPath);
    sl::separator();
    sl::child("ResourceTree", [&]() { renderResourceTree(); });
}

void ResourcesView::renderResourceTree() {
    sl::treeNode(
      ICON_FA_FOLDER "  assets", [&]() { renderNode(m_root); },
      ImGuiTreeNodeFlags_DefaultOpen
    );
}

void ResourcesView::renderNode(Node& node) {
    for (auto& child : node.children) {
        const auto flags =
          child.isDirectory()
            ? ImGuiTreeNodeFlags_DefaultOpen
            : ImGuiTreeNodeFlags_Leaf;
        sl::treeNode(
          fmt::format("{}  {}", child.thumbnail, child.name),
          [&]() { renderNode(child); }, flags
        );
        if (not child.isDirectory() && sl::wasItemClicked())
            setSelectedResource(child);
    }
}

void ResourcesView::renderUnknownResource(Resource& resource) {
    sl::text("Could not render preview, unknown resource format");
    sl::separator();
    sl::text("{}", resource.name + resource.extension);
}

void ResourcesView::setSelectedResource(Resource& node) {
    if (auto view = m_views.find(node.type); view != m_views.end())
        setInspectorCallback([&]() { view->second->render(node); });
    else
        setInspectorCallback([&]() { renderUnknownResource(node); });
}

void ResourcesView::build() {
    resetResources();
    processNode(m_root);
}

void ResourcesView::processNode(Node& node, const std::string& prefix) {
    for (const auto& item : kstd::listDirectory(node.fullPath)) {
        node.children.emplace_back(item, prefix);

        if (auto& child = node.children.back(); child.isDirectory()) {
            processNode(child, child.name);
        } else {
            addResource(child.type, child.assetPath);
        }
    }
}

}  // namespace sle
