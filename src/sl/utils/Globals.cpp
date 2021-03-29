#include "Globals.h"

#include "globals/Geometry.h"
#include "globals/Shaders.h"

namespace sl::utils {

void Globals::init() {
    shaders = std::make_unique<globals::Shaders>();
    geometry = std::make_unique<globals::Geometry>();
}

Globals& Globals::instance() {
    static Globals globals;
    return globals;
}

}