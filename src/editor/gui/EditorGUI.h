#pragma once

#include "LeftPanel.h"
#include "BottomPanel.h"
#include "Settings.h"
#include "sl/asset/AssetManager.hpp"
#include "sl/core/event/Event.h"
#include "sl/core/event/EventBus.h"
#include "sl/ecs/Entity.h"
#include "sl/gui/GUIProxy.h"
#include "sl/gui/Utils.hpp"
#include "sl/math/Vector.hpp"

using namespace sl::core;

namespace editor::gui {

class EditorGUI {

public:
    explicit EditorGUI(const Settings& settings, EntitiesVector& entities, res::ResourceManager& resourceManager)
        : m_settings(settings)
        , m_entities(entities)
        , m_leftPanel(settings, entities, resourceManager)
        , m_resourceManager(resourceManager)
        , m_entityIndex(0) {
    }

    void renderEditorGUI(sl::gui::GUIProxy& gui) {
        // clang-format off
		const int sceneEntitiesPanelHeight = m_settings.viewportHeight * m_settings.leftSideTopBottomRatio;
		const int leftPanelWidth = m_settings.viewportWidth * m_settings.leftPanelWidthRatio;

		m_leftPanel.render(gui);
		m_bottomPanel.render(gui);


		// fie fie
		auto bottomPanelHeight = 0.35f * m_settings.viewportHeight;
		gui->beginPanel(sl::gui::createHiddenLabel("BottomMenu"), 
				sl::math::Vec2{leftPanelWidth, m_settings.viewportHeight - bottomPanelHeight}, 
				sl::math::Vec2{m_settings.viewportWidth - leftPanelWidth, bottomPanelHeight});
			
			gui->beginTabBar("bottomTabBar");
			
			if (gui->beginTabItem("Resources")) {
				gui->beginGroup();
				if (gui->button("+"))
					gui->openPopUp("ResourceLoadPopUp");				
				

				if (gui->beginPopUp("ResourceLoadPopUp")) {
					static int activeItem = 0;
					std::vector<std::string> labels = {"Cubemap", "Texture", "Model"};
					gui->combo(sl::gui::createHiddenLabel("ResourcesCombo"), activeItem, labels);

					
					bool clicked = false;
					if (gui->button("Load")) {
						gui->closeCurrentPopUp();
						clicked = true;
					}

					gui->sameLine(250);
					gui->beginGroup();
		
					constexpr int padding = 65;
					gui->displayText("Name"); 
					gui->sameLine(padding);
					gui->inputText(sl::gui::createHiddenLabel("Name"), name);

					switch (activeItem) {
						case 0: {	
							gui->displayText("Top"); 
							gui->sameLine(padding);
							gui->inputText(sl::gui::createHiddenLabel("Top"), top);

							gui->displayText("Bottom"); 
							gui->sameLine(padding);
							gui->inputText(sl::gui::createHiddenLabel("Bottom"), bottom);
							
							gui->displayText("Right"); 
							gui->sameLine(padding);
							gui->inputText(sl::gui::createHiddenLabel("Right"), right);

							gui->displayText("Left"); 
							gui->sameLine(padding);
							gui->inputText(sl::gui::createHiddenLabel("Left"), left);
							
							gui->displayText("Front"); 
							gui->sameLine(padding);
							gui->inputText(sl::gui::createHiddenLabel("Front"), front);
							
							gui->displayText("Back"); 
							gui->sameLine(padding);
							gui->inputText(sl::gui::createHiddenLabel("Back"), back);
						
							if (clicked) {
								auto cubemap = sl::asset::AssetManager::load<sl::platform::texture::Cubemap>(
									right, left, top, bottom, front, back);	
								auto cubemapResource = std::make_shared<res::CubemapResource>(cubemap, name);
								m_resourceManager.addResource(cubemapResource);								
							}
						}
					}
					gui->endGroup();
					

					gui->endPopUp();
				}

				gui->endGroup();
			
				for (auto& [_, namesVector] : m_resourceManager.getAllNames())
					for (auto& resourceName : namesVector)
						gui->displayText(resourceName);
			
				gui->endTabItem();
			}

			if (gui->beginTabItem("Debug console")) {

				gui->endTabItem();
			}


			gui->endTabBar();
		gui->endPanel();

        // clang-format on
    }

private:
    LeftPanel m_leftPanel;
	BottomPanel m_bottomPanel;

    std::string name;
    std::string top = "/skybox/top.jpg";
    std::string bottom = "/skybox/bottom.jpg";
    std::string left = "/skybox/left.jpg";
    std::string right = "/skybox/right.jpg";
    std::string front = "/skybox/front.jpg";
    std::string back = "/skybox/back.jpg";

    res::ResourceManager& m_resourceManager;
    EntitiesVector& m_entities;
    Settings m_settings;

 
   };
}
