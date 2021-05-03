#pragma once

#include <concepts>
#include <memory>
#include <ranges>
#include <unordered_map>

#include "sl/core/GameObject.h"

namespace sl::asset {

template <typename T>
requires std::derived_from<T, core::GameObject> class AssetContainer {
public:
    void insert(std::shared_ptr<T> asset, const std::string& name) {
        m_nameToAsset[name] = asset;
        m_idToName[asset->id] = name;
    }

    std::unordered_map<std::string, std::shared_ptr<T>>& getAll() {
        return m_nameToAsset;
    }

    std::vector<std::string> getNames() const {
        std::vector<std::string> names;
        std::ranges::copy(m_nameToAsset | std::views::keys, std::back_inserter(names));

        return names;
    }

    bool has(int id) {
        return m_idToName.contains(id);
    }

    bool has(const std::string& name) {
        return m_nameToAsset.contains(name);
    }

    std::shared_ptr<T> getByName(const std::string& name) {
        return m_nameToAsset.at(name);
    }

    std::shared_ptr<T> getById(int id) {
        return m_nameToAsset.at(
            m_idToName.at(id));
    }

private:
    std::unordered_map<std::string, std::shared_ptr<T>> m_nameToAsset;
    std::unordered_map<int, std::string> m_idToName;
};

}