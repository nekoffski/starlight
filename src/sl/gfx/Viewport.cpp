#include "Viewport.h"

#include <kc/core/Log.h>

#include <ostream>

namespace sl::gfx {

std::string toString(const Viewport& viewport) {
    return fmt::format(
        "Viewport: x={} y={} w={} h={}", viewport.beginX, viewport.beginY, viewport.width,
        viewport.height
    );
}

std::ostream& operator<<(const Viewport& viewport, std::ostream& stream) {
    stream << toString(viewport);
    return stream;
}

}  // namespace sl::gfx