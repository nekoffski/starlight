#include "Data.hh"

#include <starlight/core/Function.hh>

namespace sle {

sl::ui::ImageHandle* Data::getTextureImage(sl::Texture& texture) {
    auto [it, _] = images.try_emplace(
      texture.id,
      sl::lazyEvaluate([&] { return sl::ui::ImageHandle::createHandle(&texture); })
    );
    return it->second.get();
}

}  // namespace sle
