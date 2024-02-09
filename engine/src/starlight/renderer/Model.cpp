#include "Model.h"

namespace sl {

Model::Model() : transform(nullptr) {}

Mat4f Model::getModelMatrix() {
    return transform ? transform->getWorld() : identityMatrix;
}

}  // namespace sl
