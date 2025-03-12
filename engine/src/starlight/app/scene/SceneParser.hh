#pragma once

#include <unordered_map>

#include "starlight/core/FileSystem.hh"
#include "starlight/core/memory/SharedPtr.hh"
#include "starlight/core/Log.hh"
#include "starlight/core/Json.hh"
#include "starlight/app/scene/Scene.hh"

namespace sl {

class SceneParser {
    using Deserializer = std::function<void(Entity&, const nlohmann::json&)>;
    using Serializer   = std::function<nlohmann::json(void*)>;

public:
    template <typename T, typename S, typename D>
    requires Callable<S, nlohmann::json, T&>
             && Callable<D, void, Entity&, const nlohmann::json&>
    SceneParser& registerComponent(
      const std::string& name, S&& serializer, D&& deserializer
    ) {
        log::expect(
          not m_deserializers.contains(name
          ) && not m_serializers.contains(typeid(T)),
          "Parser for component '{}' already registered", name
        );

        m_serializers[typeid(T)] = [s = std::move(serializer)](void* component) {
            return s(*static_cast<T*>(component));
        };
        m_deserializers[name] = std::move(deserializer);

        return *this;
    }

    explicit SceneParser(const FileSystem* fs = FileSystem::getDefaultPtr());

    void serialize(Scene& scene, const std::string& path);
    SharedPtr<Scene> deserialize(const std::string& path);

private:
    void deserializeEntity(Scene& scene, const nlohmann::json& node);
    nlohmann::json serializeEntity(Entity& entity);

    const FileSystem* m_fs;

    std::unordered_map<std::string, Deserializer> m_deserializers;
    std::unordered_map<std::type_index, Serializer> m_serializers;
};

}  // namespace sl
