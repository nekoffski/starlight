#include "Random.hh"

namespace sl {

RandomEngine::RandomEngine()
    : m_engine(std::random_device{}()), m_uuidGen(m_engine) {}

Str RandomEngine::uuid() { return uuids::to_string(m_uuidGen()); }

}  // namespace sl
