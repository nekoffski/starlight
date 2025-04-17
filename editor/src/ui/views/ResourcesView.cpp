#include "ResourcesView.hh"

#include <starlight/app/factories/MeshFactory.hh>
#include <starlight/app/factories/MaterialFactory.hh>
#include <starlight/app/factories/TextureFactory.hh>
#include <starlight/core/TaskQueue.hh>

#include "Console.hh"

namespace sle {

// static constexpr sl::u32 rowSize = 5;

// static sl::f32 getThumbnailWidth() {
//     return ImGui::GetWindowWidth() / static_cast<sl::f32>(rowSize + 1);
// }

ResourcesView::ResourcesView(Widget::State& state)
    : Widget(state) {}

void ResourcesView::render() {}

}  // namespace sle
