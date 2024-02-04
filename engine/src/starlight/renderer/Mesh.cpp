#include "Mesh.h"

namespace sl {

Mesh::Mesh() : transform(nullptr) {}

Mat4f Mesh::getModelMatrix() {
    return transform ? transform->getWorld() : identityMatrix;
}

}  // namespace sl
