#pragma once

#include <kcore/Concepts.hh>

#include "GeometryData.hh"

namespace sl {

class ProceduralGeometry : public kc::StaticClass {
   public:
    // Unit cube centered at the origin, with flat normals and per-face UVs.
    // Each face occupies four vertices and six consecutive indices.
    static GeometryData cube();
};

}  // namespace sl
