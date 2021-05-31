#pragma once

#include <string>

#include "Logger.h"
#include "Uuid.h"

namespace sl::core {

class GameObject {
public:
    GameObject()
        : m_id(generateUuid()) {
        SL_INFO("Creating GameObject: {}", m_id);
    }

    ~GameObject() {
        SL_INFO("Destroying GameObject: {}", m_id);
    }

    GameObject(GameObject&& oth) {
        m_id = std::move(oth.m_id);
    }

    const std::string getId() const {
        return m_id;
    }

protected:
    std::string m_id;
};

}