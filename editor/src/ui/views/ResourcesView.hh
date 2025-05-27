#pragma once

#include <unordered_map>

#include <starlight/ui/UI.hh>
#include <starlight/ui/widgets/Image.hh>

#include <kstd/memory/UniquePtr.hh>

#include "ui/Widget.hh"
#include "ui/Resource.hh"
#include "resources/ResourceView.hh"

namespace sle {

class ResourcesView : public Widget {
public:
    struct Node : Resource {
        using Resource::Resource;
        std::vector<Node> children;
    };

    explicit ResourcesView(Widget::State& state);

    void render();

private:
    void renderResourceTree();
    void renderNode(Node& node);
    void setSelectedResource(Resource& node);
    void renderUnknownResource(Resource& resource);

    void build();
    void processNode(Node& node, const std::string& prefix = "");

    kstd::SharedPtr<sl::Texture> m_folderTexture;
    Node m_root;

    std::unordered_map<Resource::Type, kstd::UniquePtr<ResourceView>> m_views;
};

}  // namespace sle
