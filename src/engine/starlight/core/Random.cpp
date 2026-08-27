#include "Random.hh"

namespace starlight {

RandomEngine::RandomEngine()
    : m_engine(std::random_device{}()), m_uuidGen(m_engine) {}

std::string RandomEngine::uuid() { return uuids::to_string(m_uuidGen()); }

}  // namespace starlight
