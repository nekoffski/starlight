#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "starlight/core/Core.hh"
#include "starlight/core/Id.hh"
#include "starlight/core/memory/Memory.hh"

namespace sl {

template <typename T> class ResourceManager;

template <typename T> class ResourceRef {
public:
    explicit ResourceRef(
      T* resource, ResourceManager<T>* manager, std::string_view name
    );

    ResourceRef(T* resource);
    ResourceRef(std::nullptr_t);
    ResourceRef();

    ~ResourceRef();

    operator bool() const;

    T* operator->();
    const T* operator->() const;

    T* get();
    const T* get() const;

private:
    T* m_resource;
    ResourceManager<T>* m_manager;
    std::optional<std::string> m_name;
};

template <typename T> class ResourceManager {
    friend class ResourceRef<T>;

    struct ResourceRecord {
        OwningPtr<T> data;
        u32 referenceCounter;
    };

public:
    ResourceRef<T> find(const std::string& name) {
        if (auto record = m_records.find(name); record != m_records.end())
            return ResourceRef<T>(record->second.data.get(), this, name);
        return nullptr;
    }

protected:
    ResourceRef<T> store(std::string_view name, OwningPtr<T> resource) {
        const auto [it, inserted] = m_records.emplace(name, std::move(resource));

        if (not inserted) {
            // TODO: log
            return nullptr;
        }

        return ResourceRef<T>(it->second.data.get(), this, name);
    }

    void release(const std::string& name) {
        if (auto it = m_records.find(name); it != m_records.end()) {
            if (--it->second.referenceCounter == 0) m_records.erase(name);
        } else {
            LOG_WARN("Could not find record to release with name: {}", name);
        }
    }

    void acquire(std::string_view name) {}

private:
    std::unordered_map<std::string, ResourceRecord> m_records;
};

template <typename T>
ResourceRef<T>::ResourceRef(
  T* resource, ResourceManager<T>* manager, std::string_view name
) :
    m_resource(resource),
    m_manager(manager), m_name(name) {}

template <typename T>
ResourceRef<T>::ResourceRef(T* resource) :
    m_resource(resource), m_manager(nullptr) {}

template <typename T>
ResourceRef<T>::ResourceRef(std::nullptr_t) :
    m_resource(nullptr), m_manager(nullptr) {}

template <typename T>
ResourceRef<T>::ResourceRef() : m_resource(nullptr), m_manager(nullptr) {}

template <typename T> ResourceRef<T>::~ResourceRef() {
    if (m_manager) m_manager->release(*m_name);
}

template <typename T> T* ResourceRef<T>::operator->() { return m_resource; }

template <typename T> const T* ResourceRef<T>::operator->() const {
    return m_resource;
}

template <typename T> T* ResourceRef<T>::get() { return m_resource; }
template <typename T> const T* ResourceRef<T>::get() const { return m_resource; }

template <typename T> ResourceRef<T>::operator bool() const {
    return m_resource != nullptr;
}

}  // namespace sl