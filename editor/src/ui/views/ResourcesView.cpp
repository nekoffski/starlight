#include "ResourcesView.hh"

#include <kstd/String.hh>

#include <starlight/app/factories/MeshFactory.hh>
#include <starlight/app/factories/MaterialFactory.hh>
#include <starlight/app/factories/TextureFactory.hh>
#include <starlight/core/TaskQueue.hh>

#include "Console.hh"

namespace sle {

static constexpr sl::u32 rowSize = 8u;

static sl::f32 getThumbnailWidth() {
    return ImGui::GetWindowWidth() / static_cast<sl::f32>(rowSize + 1);
}

ResourcesView::ResourcesView(Widget::State& state)
    : Widget(state)
    , m_folderTexture(sl::TextureFactory::get().loadFlat("folder.png")) {
    m_activeNode = &m_root;
    build();
}

void ResourcesView::render() {
    sl::conditionallyDisabled(
      [&]() {
          if (sl::button(ICON_FA_BACKWARD)) m_activeNode = m_activeNode->parent;
      },
      m_activeNode->parent == nullptr
    );

    sl::sameLine();

    if (sl::button("New")) {
    }

    sl::sameLine();
    sl::conditionallyDisabled(
      [&]() {
          if (sl::button("Delete")) {
          }
      },
      true
    );

    sl::sameLine();
    sl::text("  {}", m_activeNode->fullPath);

    sl::separator();
    renderResourceTree();
}

void ResourcesView::renderResourceTree() {
    const auto w = getThumbnailWidth();

    for (sl::u32 i = 0; i < m_activeNode->children.size(); ++i) {
        auto& child = m_activeNode->children[i];

        ImVec2 padding(4, 4);
        ImVec2 imageSize(w, w);
        ImVec2 textSize = ImGui::CalcTextSize(child.name.c_str());
        float spacing   = ImGui::GetStyle().ItemSpacing.y;

        ImVec2 groupSize(
          std::max(imageSize.x, textSize.x) + padding.x * 2,
          imageSize.y + spacing + textSize.y + padding.y * 2
        );

        if (i % rowSize == 0) ImGui::NewLine();

        ImVec2 startPos = ImGui::GetCursorScreenPos();

        if (ImGui::InvisibleButton(("hover_" + child.name).c_str(), groupSize)) {
            if (child.type == ResourceType::directory) {
                m_activeNode = &child;
                return;
            } else {
            }
        }
        bool isHovered = ImGui::IsItemHovered();
        ImGui::SameLine();

        if (isHovered) {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            drawList->AddRectFilled(
              startPos, ImVec2(startPos.x + groupSize.x, startPos.y + groupSize.y),
              IM_COL32(100, 100, 255, 165), 5.0f
            );
        }

        ImGui::SetCursorScreenPos(
          ImVec2(startPos.x + padding.x, startPos.y + padding.y)
        );
        sl::group([&]() {
            showImage(*m_folderTexture, w);
            sl::text("{}{}", sl::spaces(2u), child.name);
        });

        sl::sameLine();
    }
}

void ResourcesView::build() {
    m_root.parent   = nullptr;
    m_root.name     = "Assets";
    m_root.fullPath = getConfig().assetsRoot;
    m_root.type     = ResourceType::directory;

    auto& fs = kstd::GlobalFileSystem::get();

    for (const auto& item : fs.listDirectory(m_root.fullPath)) {
        if (fs.isDirectory(item)) {
            m_root.children.emplace_back(
              &m_root, ResourceType::directory, kstd::nameFromPath(item), item
            );
        }
    }
}

}  // namespace sle
