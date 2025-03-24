#include "Model.hh"

namespace sl {

Model::Model(OptStr name) : NamedResource(name) {}

Transform& Model::getTransform() { return m_transform; }

}  // namespace sl
