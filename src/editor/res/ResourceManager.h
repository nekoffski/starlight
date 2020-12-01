#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Resource.h"

namespace editor::res {

class ResourceManager {
    using NameToResourceMap = std::unordered_map<std::string, std::shared_ptr<Resource>>;
    using TypeToResourceMapMap = std::unordered_map<ResourceType, NameToResourceMap>;
	using NamesVector = std::vector<std::string>;
	using TypeToNamesMap = std::unordered_map<ResourceType, NamesVector>;

public:
    TypeToResourceMapMap& getAllResources() {
        return m_resources;
    }

    NameToResourceMap& getResourcesByType(ResourceType type) {
        if (m_resources.contains(type))
            return m_resources[type];
        return m_emptyResourceMap;
    }

	TypeToNamesMap& getAllNames() {
		return m_resourceNames;
	}

	NamesVector& getNamesByType(ResourceType type) {
		if (m_resourceNames.contains(type))
			return m_resourceNames[type];
		return m_emptyNamesVector;
	}

    void addResource(std::shared_ptr<Resource> resource) {
		auto type = resource->getType();
		m_resourceNames[type].push_back(resource->name);
		m_resources[type][resource->name] = resource;
    }

private:
	TypeToResourceMapMap m_resources;
	TypeToNamesMap m_resourceNames;

	inline static NamesVector m_emptyNamesVector;
    inline static NameToResourceMap m_emptyResourceMap;
};
}
