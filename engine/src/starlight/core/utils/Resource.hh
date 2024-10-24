#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "starlight/core/Core.hh"
#include "starlight/core/utils/Id.hh"
#include "starlight/core/memory/Memory.hh"

namespace sl {

template <typename T> class ResourceManager;

template <typename T> class ResourceRef {
public:
    explicit ResourceRef(
      T* resource, ResourceManager<T>* manager, const std::string& name
    );

    ResourceRef(T* resource);
    ResourceRef(std::nullptr_t);
    ResourceRef();

    ResourceRef(const ResourceRef& oth);
    ResourceRef(ResourceRef&& oth);

    ResourceRef& operator=(const ResourceRef& oth);
    ResourceRef& operator=(ResourceRef&& oth);

    ~ResourceRef();

    operator bool() const;

    operator T*();
    operator T*() const;

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
        std::string name;
        u32 referenceCounter;
    };

public:
    ResourceRef<T> find(const std::string& name) {
        if (auto record = m_records.find(name); record != m_records.end())
            return ResourceRef<T>(record->second.data.get(), this, name);
        return nullptr;
    }

    template <typename U = T>
    requires std::is_same_v<U, T> && IsIdentificable<U>
    ResourceRef<T> find(const u64 id) {
        if (auto record = m_recordsById.find(id); record != m_recordsById.end()) {
            auto resource = record->second;
            return ResourceRef<T>(resource->data.get(), this, resource->name);
        }
        return nullptr;
    }

protected:
    ResourceRef<T> store(const std::string& name, OwningPtr<T> resource) {
        const auto [it, inserted] =
          m_records.emplace(name, ResourceRecord{ std::move(resource), name, 0u });

        if (not inserted) {
            LOG_WARN("Record with name='{}' already exists", name);
            return nullptr;
        }

        auto& record = it->second;

        if constexpr (IsIdentificable<T>) {
            const auto id            = record.data->getId();
            const auto [_, inserted] = m_recordsById.emplace(id, &record);

            ASSERT(inserted, "Map 'id' -> 'record' desynchronized with main buffer");
        }
        return ResourceRef<T>(record.data.get(), this, name);
    }

    void release(const std::string& name) {
        LOG_DEBUG("Releasing resource: {}", name);

        if (auto it = m_records.find(name); it != m_records.end()) {
            if (auto& record = it->second; --record.referenceCounter <= 0) {
                LOG_INFO(
                  "Reference counter of Resource {} less or equals 0, destroying",
                  name
                );
                if constexpr (IsIdentificable<T>)
                    m_recordsById.erase(record.data->getId());
                m_records.erase(name);
            } else {
                LOG_DEBUG("{} references left", record.referenceCounter);
            }
        } else {
            LOG_WARN("Could not find record to release with name: {}", name);
        }
    }

    bool acquire(const std::string& name) {
        if (auto record = m_records.find(name); record != m_records.end()) {
            record->second.referenceCounter++;
            return true;
        }
        return false;
    }

private:
    std::unordered_map<std::string, ResourceRecord> m_records;
    std::unordered_map<u64, ResourceRecord*> m_recordsById;
};

template <typename T>
ResourceRef<T>::ResourceRef(
  T* resource, ResourceManager<T>* manager, const std::string& name
) : m_resource(resource), m_manager(manager), m_name(name) {
    LOG_TRACE("Creating resource ref: {}", name);
    ASSERT(
      resource != nullptr,
      "Attempt to create ResourceRef with resource == nullptr, {}", name
    );
    ASSERT(
      manager != nullptr,
      "Attempt to create ResourceRef with manager == nullptr, {}", name
    );
    ASSERT(manager->acquire(name), "Could not acquire resource: {}", name);
}

template <typename T>
ResourceRef<T>::ResourceRef(T* resource
) : m_resource(resource), m_manager(nullptr) {}

template <typename T>
ResourceRef<T>::ResourceRef(std::nullptr_t
) : m_resource(nullptr), m_manager(nullptr) {}

template <typename T>
ResourceRef<T>::ResourceRef() : m_resource(nullptr), m_manager(nullptr) {}

template <typename T> ResourceRef<T>::~ResourceRef() {
    if (m_manager) m_manager->release(*m_name);
}

template <typename T>
ResourceRef<T>::ResourceRef(const ResourceRef<T>& oth
) : m_resource(oth.m_resource), m_manager(oth.m_manager), m_name(oth.m_name) {
    if (m_manager) m_manager->acquire(*m_name);
}

template <typename T>
ResourceRef<T>::ResourceRef(ResourceRef&& oth) :
    m_resource(std::exchange(oth.m_resource, nullptr)),
    m_manager(std::exchange(oth.m_manager, nullptr)),
    m_name(std::exchange(oth.m_name, std::nullopt)) {}

template <typename T>
ResourceRef<T>& ResourceRef<T>::operator=(const ResourceRef<T>& oth) {
    if (m_manager) m_manager->release(*m_name);

    m_manager  = oth.m_manager;
    m_resource = oth.m_resource;
    m_name     = oth.m_name;

    if (m_manager) m_manager->acquire(*m_name);

    return *this;
}

template <typename T>
ResourceRef<T>& ResourceRef<T>::operator=(ResourceRef<T>&& oth) {
    m_manager     = oth.m_manager;
    m_resource    = oth.m_resource;
    m_name        = oth.m_name;
    oth.m_manager = nullptr;
    return *this;
}

template <typename T> T* ResourceRef<T>::operator->() { return m_resource; }

template <typename T> const T* ResourceRef<T>::operator->() const {
    return m_resource;
}

template <typename T> T* ResourceRef<T>::get() { return m_resource; }
template <typename T> const T* ResourceRef<T>::get() const { return m_resource; }

template <typename T> ResourceRef<T>::operator T*() const { return m_resource; }
template <typename T> ResourceRef<T>::operator T*() { return m_resource; }

template <typename T> ResourceRef<T>::operator bool() const {
    return m_resource != nullptr;
}

}  // namespace sl