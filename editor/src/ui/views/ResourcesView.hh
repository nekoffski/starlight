#pragma once

#include <unordered_map>

#include <starlight/ui/UI.hh>
#include <starlight/ui/widgets/Image.hh>

#include "ui/Widget.hh"
#include "resources/ResourceView.hh"

namespace sle {

class ResourcesView : public Widget {
public:
    struct Node {
        ResourceType type;
        std::string name;
        std::string fullPath;
        std::string extension;
        bool isDirectory;
        std::vector<Node> children;
    };

    explicit ResourcesView(Widget::State& state);

    void render();

private:
    void renderResourceTree();
    void renderNode(Node& node);

    void build();
    void processNode(Node& node, const kstd::FileSystem& fs);

    kstd::SharedPtr<sl::Texture> m_folderTexture;
    Node m_root;
    Node* m_activeNode;
};

}  // namespace sle
