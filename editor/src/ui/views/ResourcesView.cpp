#include "ResourcesView.hh"

#include <kstd/String.hh>

#include <starlight/app/factories/MeshFactory.hh>
#include <starlight/app/factories/MaterialFactory.hh>
#include <starlight/app/factories/TextureFactory.hh>
#include <starlight/core/TaskQueue.hh>

#include "Console.hh"

namespace sle {

static ResourceType extensionToResourceType(const std::string& extension) {
    if (extension == ".tga" || extension == ".jpg" || extension == ".png")
        return ResourceType::texture;
    else if (extension == ".obj")
        return ResourceType::wavefrontObject;
    else if (extension == ".mtl")
        return ResourceType::wavefrontMaterial;
    else if (extension == ".vert" || extension == ".spv" || extension == ".frag")
        return ResourceType::shader;
    return ResourceType::unknown;
}

static std::string_view getResourceThumbnail(ResourceType type) {
    switch (type) {
        case ResourceType::directory:
            return ICON_FA_FOLDER;
        case ResourceType::texture:
            return ICON_FA_IMAGE;
        case ResourceType::shader:
            return ICON_FA_LIGHTBULB;
        case ResourceType::wavefrontMaterial:
            return ICON_FA_FEATHER;
        case ResourceType::wavefrontObject:
            return ICON_FA_CUBE;
        case ResourceType::unknown:
        default:
            return ICON_FA_FILE;
    }
}

ResourcesView::ResourcesView(Widget::State& state)
    : Widget(state)
    , m_folderTexture(sl::TextureFactory::get().loadFlat("folder.png")) {
    m_activeNode = &m_root;
    build();
}

void ResourcesView::render() {
    sl::text("Project path: {}", m_activeNode->fullPath);
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
          child.isDirectory
            ? ImGuiTreeNodeFlags_DefaultOpen
            : ImGuiTreeNodeFlags_Leaf;
        sl::treeNode(
          fmt::format("{}  {}", getResourceThumbnail(child.type), child.name),
          [&]() { renderNode(child); }, flags
        );
    }
}

void ResourcesView::build() {
    m_root.name     = "Assets";
    m_root.fullPath = getConfig().assetsRoot;
    m_root.type     = ResourceType::directory;

    auto& fs = kstd::GlobalFileSystem::get();
    processNode(m_root, fs);
}

void ResourcesView::processNode(Node& node, const kstd::FileSystem& fs) {
    for (const auto& item : fs.listDirectory(node.fullPath)) {
        bool isDirectory = fs.isDirectory(item);
        const auto extension =
          kstd::extensionFromPath(item, kstd::ExtensionExtractionMode::lastChunk)
            .value_or("");
        node.children.emplace_back(
          isDirectory ? ResourceType::directory : extensionToResourceType(extension),
          kstd::nameFromPath(item, kstd::NameExtractionMode::withExtension), item,
          extension, isDirectory
        );
        if (isDirectory) processNode(node.children.back(), fs);
    }
}

}  // namespace sle
