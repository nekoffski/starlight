#pragma once

#include <unordered_map>

#include <kstd/serialization/All.hh>

#include "starlight/core/Log.hh"
#include "starlight/core/Json.hh"
#include "starlight/app/scene/Scene.hh"

#include "ComponentParser.hh"

namespace sl {

class SceneParser {
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

    explicit SceneParser();

    void serialize(Scene& scene, const std::string& path);
    kstd::SharedPtr<Scene> deserialize(const std::string& path);

private:
    void deserializeEntity(Scene& scene, const nlohmann::json& node);
    void serializeEntity(Entity& entity);

    bool deserializeComponent(
      const std::string& name, Entity& entity, const nlohmann::json& node
    );

    std::unordered_map<std::type_index, std::string> m_componentToName;
    std::unordered_map<std::string, std::unique_ptr<ComponentParser>>
      m_componentParsers;
};

}  // namespace sl
