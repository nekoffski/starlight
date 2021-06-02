#pragma once

#include <memory>

namespace sl::core {

template <typename T>
class OptionalPtr {
public:
    OptionalPtr()
        : m_data(nullptr) {
    }

    OptionalPtr(std::shared_ptr<T>& data)
        : m_data(data.get()) {
    }

    OptionalPtr(std::unique_ptr<T>& data)
        : m_data(data.get()) {
    }

    OptionalPtr(T* data)
        : m_data(data) {
    }

    OptionalPtr& operator=(std::unique_ptr<T>& data) {
        return operator=(data.get());
    }

    OptionalPtr& operator=(std::shared_ptr<T>& data) {
        return operator=(data.get());
    }

    OptionalPtr& operator=(T* data) {
        m_data = data;

        return *this;
    }

    void reset() {
        m_data = nullptr;
    }

    T* operator->() {
        return m_data;
    }

    T& value() {
        return *m_data;
    }

    const T& value() const {
        return *m_data;
    }

    T& operator*() {
        return value();
    }

    const T& operator*() const {
        return value();
    }

    bool hasValue() const {
        return m_data != nullptr;
    }

    operator bool() const {
        return hasValue();
    }

private:
    T* m_data;
};

}