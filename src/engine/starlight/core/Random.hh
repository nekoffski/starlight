#pragma once

#include <uuid.h>

#include <random>

#include "Core.hh"
#include "Singleton.hh"

namespace sl {

class RandomEngine : public Singleton<RandomEngine> {
   public:
    explicit RandomEngine();

    std::string uuid();

   private:
    std::mt19937 m_engine;
    uuids::uuid_random_generator m_uuidGen;
};

}  // namespace sl
