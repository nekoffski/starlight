#pragma once

#include "starlight/core/utils/Resource.hh"

#include "gpu/Mesh.hh"
#include "Material.hh"

namespace sl {

struct MeshTree {
    ResourceRef<Mesh> mesh;
    ResourceRef<Material> material;
};

}  // namespace sl
