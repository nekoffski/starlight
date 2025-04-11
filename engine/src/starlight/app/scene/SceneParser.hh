#pragma once

#include <unordered_map>

#include "starlight/core/FileSystem.hh"
#include "starlight/core/memory/SharedPtr.hh"
#include "starlight/core/Log.hh"
#include "starlight/core/Json.hh"
#include "starlight/app/scene/Scene.hh"

#include "ComponentParser.hh"

namespace sl {

class SceneParser {
    using Deserializer = std::function<void(Entity&, const nlohmann::json&)>;
    using Serializer   = std::function<nlohmann::json(void*)>;

public:
    template <typename T, typename Parser>
    requires(std::is_base_of_v<Component, T> && std::is_base_of_v<ComponentParser, Parser>)
    SceneParser& registerParser() {
        log::expect(
          not m_componentToName.contains(typeid(T)),
          "Parser for component '{}' already registered", typeid(T).name()
        );

        auto parser                  = std::make_unique<Parser>();
        const auto name              = parser->getComponentName();
        m_componentToName[typeid(T)] = name;
        m_componentParsers[name]     = std::move(parser);

        return *this;
    }

    explicit SceneParser(const FileSystem* fs = FileSystem::getDefaultPtr());

    void serialize(Scene& scene, const std::string& path);
    SharedPtr<Scene> deserialize(const std::string& path);

private:
    void deserializeEntity(Scene& scene, const nlohmann::json& node);
    nlohmann::json serializeEntity(Entity& entity);

    bool deserializeComponent(
      const std::string& name, Entity& entity, const nlohmann::json& node
    );

    const FileSystem* m_fs;

    std::unordered_map<std::type_index, std::string> m_componentToName;
    std::unordered_map<std::string, std::unique_ptr<ComponentParser>>
      m_componentParsers;
};

}  // namespace sl
