#pragma once

#include <kc/core/Log.h>
#include <kc/core/Uuid.h>

#include <memory>
#include <string>

#include "Errors.hpp"

namespace sl::core {

class GameObject {
   public:
    GameObject() : m_id(kc::core::generateUuid()) { LOG_INFO("Creating GameObject: {}", m_id); }

    ~GameObject() { LOG_INFO("Destroying GameObject: {}", m_id); }

    GameObject(GameObject&& oth) { m_id = std::move(oth.m_id); }

    const std::string getId() const { return m_id; }

   protected:
    std::string m_id;
};

}  // namespace sl::core