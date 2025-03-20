#include "Config.hh"

namespace sle {

Config Config::createDefault() {
    return Config{
        .layoutSizeRatio = { 0.20f, 0.25f }
    };
}

}  // namespace sle
