#include "PolygonMode.hh"

#include "starlight/core/Log.hh"

namespace sl {

std::string polygonModeToString(PolygonMode polygonMode) {
    switch (polygonMode) {
        case PolygonMode::fill:
            return "fill";
        case PolygonMode::line:
            return "line";
        case PolygonMode::point:
            return "point";
    }
    __builtin_unreachable();
}

PolygonMode polygonModeFromString(const std::string& polygonName) {
    if (polygonName == "line")
        return PolygonMode::line;
    else if (polygonName == "fill")
        return PolygonMode::fill;
    else if (polygonName == "point")
        return PolygonMode::point;
    FATAL_ERROR("Could not parse polygon mode: {}", polygonName);
}

}  // namespace sl