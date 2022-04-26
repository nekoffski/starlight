#include "Globals.h"

#include "Geometry.h"
#include "Shaders.h"

namespace sl::glob {

void Globals::init() {
    shaders = std::make_unique<Shaders>();
    geom    = std::make_unique<Geometry>();
}

}  // namespace sl::glob