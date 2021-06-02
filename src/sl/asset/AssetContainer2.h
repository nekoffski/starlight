#pragma once

#include <concepts>
#include <memory>
#include <ranges>
#include <unordered_map>

#include "sl/core/GameObject.h"

namespace sl::asset {

template <typename T>
requires std::derived_from<T, core::GameObject> class AssetContainer2 {
public:
    void insert(std::unique_ptr<T> asset, const std::string& name) {
        m_idToName[asset->getId()] = name;
        m_nameToAsset[name] = std::move(asset);
    }

    std::unordered_map<std::string, std::unique_ptr<T>>& getAll() {
        return m_nameToAsset;
    }

    std::vector<std::string> getNames() const {
        std::vector<std::string> names;
        std::ranges::copy(m_nameToAsset | std::views::keys, std::back_inserter(names));

        return names;
    }

    bool hasId(const std::string& id) {
        return m_idToName.contains(id);
    }

    bool hasName(const std::string& name) {
        return m_nameToAsset.contains(name);
    }

    T* getByName(const std::string& name) {
        return m_nameToAsset.at(name).get();
    }

    T* getById(const std::string& id) {
        return m_nameToAsset.at(m_idToName.at(id)).get();
    }

    void clear() {
        m_nameToAsset.clear();
        m_idToName.clear();
    }

private:
    std::unordered_map<std::string, std::unique_ptr<T>> m_nameToAsset;
    std::unordered_map<std::string, std::string> m_idToName;
};

}