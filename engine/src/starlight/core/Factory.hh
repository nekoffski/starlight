#pragma once

#include <unordered_map>

#include "Singleton.hh"
#include "Id.hh"
#include "Concepts.hh"
#include "memory/SharedPtr.hh"

namespace sl {

template <typename CFactory, typename T, typename Tag = u64>
requires HasName<T>
class Factory : public Singleton<CFactory> {
public:
    using Keys       = std::vector<std::string_view>;
    using Values     = std::vector<SharedPtr<T>>;
    using KeysView   = std::span<std::string_view>;
    using ValuesView = std::span<SharedPtr<T>>;

    SharedPtr<T> find(const std::string& key) {
        if (auto record = m_lut.find(key); record != m_lut.end())
            return record->second;
        return nullptr;
    }

    void erase(const std::string& key) {
        log::debug("Erasing '{}' from factory", key);
        m_lut.erase(key);
    }

    KeysView getKeys(Tag tag = 0u) { return m_keys[tag]; }
    ValuesView getValues(Tag tag = 0u) { return m_values[tag]; }

protected:
    SharedPtr<T> save(SharedPtr<T> resource, Tag tag = 0u) {
        const auto name = resource->name;

        // TODO: do not access by hash everytime, this requires SharedPtr fix first
        m_lut.insert({ name, std::move(resource) });
        m_keys[tag].push_back(name);
        m_values[tag].push_back(m_lut[name]);
        return m_lut[name];
    }

private:
    std::unordered_map<Tag, Values> m_values;
    std::unordered_map<Tag, Keys> m_keys;
    std::unordered_map<std::string, SharedPtr<T>> m_lut;
};

}  // namespace sl
