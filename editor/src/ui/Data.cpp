#include "Data.hh"

#include <starlight/core/Function.hh>

namespace sle {

Config Config::createDefault() {
    return Config{ .panelWidthRatio = 0.20f, .panelHeightRatio = 0.25f };
}

sl::ui::ImageHandle* Data::getTextureImage(sl::Texture& texture) {
    auto [it, _] = images.try_emplace(
      texture.id,
      sl::lazyEvaluate([&] { return sl::ui::ImageHandle::createHandle(&texture); })
    );
    return it->second.get();
}

}  // namespace sle
