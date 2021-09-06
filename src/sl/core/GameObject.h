#pragma once

#include <memory>
#include <string>

#include <kc/core/Uuid.h>

#include "Errors.hpp"
#include <kc/core/Log.h>
#include "OptionalPtr.hpp"

namespace sl::core {

class GameObject {
public:
    GameObject()
        : m_id(kc::core::generateUuid()) {
        LOG_INFO("Creating GameObject: {}", m_id);
    }

    ~GameObject() {
        LOG_INFO("Destroying GameObject: {}", m_id);
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

template <typename T>
requires std::derived_from<T, GameObject> class GameObjectPtr : public OptionalPtr<T> {
public:
    using OptionalPtr<T>::OptionalPtr;

    GameObjectPtr(std::unique_ptr<T>& data)
        : GameObjectPtr(data.get()) {
    }

    GameObjectPtr(T* data)
        : OptionalPtr<T>(data)
        , m_id(data->getId()) {
    }

    GameObjectPtr& operator=(std::unique_ptr<T>& data) {
        return operator=(data.get());
    }

    GameObjectPtr& operator=(std::shared_ptr<T>& data) {
        return operator=(data.get());
    }

    GameObjectPtr& operator=(T* data) {
        OptionalPtr<T>::operator=(data);
        m_id = data->getId();

        return *this;
    }

    std::string getId() {
        if (not m_id.has_value())
            throw NullValueAccessError { "Attempt to access null optional id of GameObjectPtr" };
        return m_id.value();
    }

    void reset() {
        OptionalPtr<T>::reset();
        m_id.reset();
    }

private:
    std::optional<std::string> m_id;
};

}