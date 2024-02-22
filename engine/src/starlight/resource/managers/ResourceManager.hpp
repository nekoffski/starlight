#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "starlight/core/utils/Log.h"

namespace sl {

template <typename Resource, typename Manager>
class ResourceManager : public kc::core::Singleton<Manager> {
public:
    explicit ResourceManager(const std::string& resourceName) :
        m_resourceName(resourceName) {}

    virtual ~ResourceManager() {
        m_idByName.clear();
        m_nameById.clear();
        m_resourceById.clear();
        m_resourceByName.clear();
    }

    template <typename Callback>
    requires Callable<Callback, void, std::string, Resource*>
    void forEach(Callback&& callback) {
        for (const auto [name, resource] : m_resourceByName)
            callback(name, resource);
    }

    template <typename Callback>
    requires Callable<Callback, void, u64, Resource*>
    void forEach(Callback&& callback) {
        for (const auto [name, id] : m_resourceById) callback(name, id);
    }

    bool has(u64 id) const { return m_resourceById.contains(id); }
    bool has(const std::string& name) { return m_resourceByName.contains(name); }

    Resource* acquire(u64 id) {
        if (auto it = m_resourceById.find(id); it != m_resourceById.end())
            return it->second;
        LOG_WARN("{} with id={} not found", m_resourceName, id);
        return nullptr;
    }

    Resource* acquire(const std::string& name) {
        if (auto it = m_resourceByName.find(name); it != m_resourceByName.end())
            return it->second;
        LOG_WARN("{} with name='{}' not found", m_resourceName, name);
        return nullptr;
    }

    void destroy(Resource* resource) { destroy(resource->getId()); }

    void destroy(u64 id) {
        if (auto it = m_resourceById.find(id); it != m_resourceById.end()) {
            destroyInternals(it->second);
            clearMapEntries(id);
        } else {
            LOG_WARN(
              "Tried to destroy '{}' with id={} which is not stored", m_resourceName,
              id
            );
        }
    }

    void destroy(const std::string& name) {
        if (auto it = m_resourceByName.find(name); it != m_resourceByName.end()) {
            destroyInternals(it->second);
            clearMapEntries(name);
        } else {
            LOG_WARN(
              "Tried to destroy '{}' with name='{}' which is not stored",
              m_resourceName, name
            );
        }
    }

    std::ranges::view auto getNames() const {
        return m_resourceByName | std::views::keys;
    }

protected:
    // TODO: if causes performance issues, optimize
    void clearMapEntries(u64 id) { clearMapEntries(m_nameById[id]); }

    void clearMapEntries(const std::string& name) {
        const auto id = m_idByName[name];

        m_resourceById.erase(id);
        m_resourceByName.erase(name);
        m_nameById.erase(id);
        m_idByName.erase(name);
    }

    Resource* storeResource(const std::string& name, u64 id, Resource* resource) {
        LOG_TRACE("Storing '{}': name='{}', id={}", m_resourceName, name, id);

        m_idByName[name]       = id;
        m_nameById[id]         = name;
        m_resourceById[id]     = resource;
        m_resourceByName[name] = resource;

        return resource;
    }

    virtual void destroyInternals(Resource* resource) = 0;

private:
    std::string m_resourceName;

    std::unordered_map<u64, Resource*> m_resourceById;
    std::unordered_map<std::string, Resource*> m_resourceByName;
    std::unordered_map<std::string, u64> m_idByName;
    std::unordered_map<u64, std::string> m_nameById;
};

}  // namespace sl
