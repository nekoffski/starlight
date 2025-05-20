#include "BoundingVolumeComponentView.hh"

#include <starlight/math/Utils.hh>
#include <starlight/physx/bvh/AABB.hh>

namespace sle {

void BoundingVolumeComponentView::render(sl::BoundingVolumeComponent& c) {
    auto& component = c.data();
    auto bv         = component.boundingVolume.get();

    sl::treeNode(
      ICON_FA_BOX "  BoundingVolume",
      [&]() {
          if (bv->is<sl::AABB>()) {
              sl::text("Type:");
              sl::text("\tAxis-Aligned Bounding Box");

              auto aabb = bv->as<sl::AABB>();
              sl::text("Extent:");
              sl::text("\t{} - {}", aabb->getMin(), aabb->getMax());
          }
      },
      ImGuiTreeNodeFlags_DefaultOpen
    );
}

}  // namespace sle
