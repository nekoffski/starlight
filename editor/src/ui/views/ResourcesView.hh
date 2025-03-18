#pragma once

#include <unordered_map>

#include <starlight/ui/UI.hh>
#include <starlight/ui/widgets/Image.hh>

#include "ui/Data.hh"

namespace sle {

class ResourcesView {
public:
    explicit ResourcesView(Data& data);

    void render();

private:
    Data& m_data;
    void renderTexturesTab();
    void renderMeshesTab();
    void renderMaterialsTab();
    void renderCubemapsTab();
    void renderModelsTab();

    void showTexture(sl::Texture& texture, sl::f32 width);

    sl::ui::TabMenu m_tabMenu;
};

}  // namespace sle
