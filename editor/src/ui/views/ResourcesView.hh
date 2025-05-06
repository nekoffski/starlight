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
        Node* parent;
        ResourceType type;
        std::string name;
        std::string fullPath;
        std::vector<Node> children;
    };

    explicit ResourcesView(Widget::State& state);

    void render();

private:
    void renderResourceTree();

    void build();

    kstd::SharedPtr<sl::Texture> m_folderTexture;
    Node m_root;
    Node* m_activeNode;
};

}  // namespace sle
