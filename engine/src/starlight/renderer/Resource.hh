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
        std::string_view name;
        u32 referenceCounter;
    };

public:
    ResourceRef<T> find(const std::string& name) {
        if (auto record = m_records.find(name); record != m_records.end())
            return ResourceRef<T>(record->second.data.get(), this, name);
        return nullptr;
    }

    template <typename U = T>
    requires std::is_same_v<U, T> && IsIdentificable<U> ResourceRef<T> find(
      const u64 id
    ) {
        if (auto record = m_recordsById.find(id); record != m_recordsById.end()) {
            auto resource = record->second;
            return ResourceRef<T>(resource->data.get(), this, resource->name);
        }
        return nullptr;
    }

protected:
    ResourceRef<T> store(std::string_view name, OwningPtr<T> resource) {
        const auto [it, inserted] =
          m_records.emplace(name, ResourceRecord{ std::move(resource), name, 0u });

        if (not inserted) {
            LOG_WARN("Record with name='{}' already exists", name);
            return nullptr;
        }

        auto& record = it->second;

        if constexpr (IsIdentificable<T>) {
            // TODO: check if already exists
            const auto id            = record.data->getId();
            const auto [_, inserted] = m_recordsById.emplace(id, &record);

            if (not inserted) {
                LOG_WARN(
                  "Record with name='{}', and id='{}' already exists", name, id
                );
                return nullptr;
            }
        }
        return ResourceRef<T>(record.data.get(), this, name);
    }

    void release(const std::string& name) {
        if (auto it = m_records.find(name); it != m_records.end()) {
            if (auto& record = it->second; --record.referenceCounter == 0) {
                if constexpr (IsIdentificable<T>)
                    m_recordsById.erase(record.data->getId());
                m_records.erase(name);
            }
        } else {
            LOG_WARN("Could not find record to release with name: {}", name);
        }
    }

    void acquire(std::string_view name) {}

private:
    std::unordered_map<std::string, ResourceRecord> m_records;
    std::unordered_map<u64, ResourceRecord*> m_recordsById;
};

template <typename T>
ResourceRef<T>::ResourceRef(
  T* resource, ResourceManager<T>* manager, std::string_view name
) :
    m_resource(resource),
    m_manager(manager), m_name(name) {
    LOG_TRACE("Creating resource ref: {}", name);
}

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