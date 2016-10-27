#include "MainGUI.h"
#include "Fleet.h"

/************************************************************************/
/* GUI                                                                  */
/************************************************************************/
MainGUI::MainGUI(){ /* Empty */ }

MainGUI::~MainGUI(){ /* Empty */ }

void MainGUI::destroyComponents(){
	m_resumeButton.destroy();
	m_optionsButton.destroy();
	m_quitButton.destroy();
}

void MainGUI::update(Sakura::InputManager& inputManager){
	m_GUICamera.update();
	switch (state){
	case GAMEPLAY:
		for (int i = 0; i < 10; ++i){
			m_shipIcons[i].unlocked = (m_shipIcons[i].cost <= (m_CP - m_usedCP)) ? true : false;
		}
		break;
	case MENU:
		m_resumeButton.update(inputManager, m_GUICamera);
		m_optionsButton.update(inputManager, m_GUICamera);
		m_quitButton.update(inputManager, m_GUICamera);
		break;
	case OPTIONSmain:
		m_quitButton.update(inputManager, m_GUICamera);
		break;
	default:
		std::printf("Gui has no state!\n");
		break;
	}
}

bool MainGUI::updateIcons(Sakura::InputManager& inputManager, const glm::vec2& mouseCoords, Fleet* pFleet, bool& placingShips){
	for (int i = 0; i < 10; ++i){
		if (m_shipIcons[i].rect.pointIntersection(mouseCoords.x, mouseCoords.y)){
			m_selectedShipType = m_shipIcons[i].shipType;
			pFleet->setAddedShip(m_shipIcons[i].shipType);
			if (!inputManager.isKeyDown(MouseId::BUTTON_LEFT) && inputManager.wasKeyDown(MouseId::BUTTON_LEFT)){
				placingShips = true;
				return true;
			}
			return false;
		}
	}
	return false;
}

void MainGUI::IDraw(float fps){
	switch (state){
	case GAMEPLAY:
		for (int i = 0; i < 10; ++i){
			m_GUISpritebatch.draw(glm::vec4(m_shipIcons[i].rect.x1, m_shipIcons[i].rect.y2, m_shipIcons[i].rect.width, m_shipIcons[i].rect.height), m_shipIconTextures.getUVs(i + (10 * (int)!m_shipIcons[i].unlocked)), m_shipIconTextures.texture.id, 50.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
			m_GUISpritefont.draw(m_GUISpritebatch, std::to_string(m_shipIcons[i].cost).c_str(), glm::vec2(m_shipIcons[i].rect.x1 + m_shipIcons[i].rect.width / 2, m_shipIcons[i].rect.y2 + 2.0f),
				glm::vec2(0.2f), 100.0f, Sakura::ColorRGBA8(255, 255, 255, 255), Sakura::Justification::MIDDLE);
		}
		m_GUISpritebatch.draw(glm::vec4(m_shipIcons[(int)m_selectedShipType].rect.x1 + 5.0f, m_shipIcons[(int)m_selectedShipType].rect.y2 + (m_shipIcons[(int)m_selectedShipType].rect.height / 2) - m_shipSelector.texture.height * 1.5f / 2,
			m_shipSelector.texture.width * 0.5f, m_shipSelector.texture.height * 1.5f), m_shipSelector.getUVs(0), m_shipSelector.texture.id, 100.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		m_GUISpritebatch.draw(glm::vec4(m_shipIcons[(int)m_selectedShipType].rect.x1 + m_shipIcons[(int)m_selectedShipType].rect.width - m_shipSelector.texture.width * 0.5f - 5.0f, m_shipIcons[(int)m_selectedShipType].rect.y2 + (m_shipIcons[(int)m_selectedShipType].rect.height / 2) - m_shipSelector.texture.height * 1.5f / 2,
			m_shipSelector.texture.width * 0.5f, m_shipSelector.texture.height * 1.5f), m_shipSelector.getUVs(1), m_shipSelector.texture.id, 100.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
#define cp_icon_scale 1.96f
		for (int cp = 0; cp < m_CP; ++cp){
			int uv = (cp <  (m_usedCP + m_shipIcons[(int)m_selectedShipType].cost)) ? 1 : 2;
			uv = (cp < m_usedCP) ? 0 : uv;
			m_GUISpritebatch.draw(glm::vec4(10.0f + (cp * (m_CPIcon.texture.width / m_CPIcon.dims.x) * cp_icon_scale), m_parentWindow->getScreenHeight() - 5.0f - (m_CPIcon.texture.width / m_CPIcon.dims.x) * cp_icon_scale, (m_CPIcon.texture.width / m_CPIcon.dims.x) * cp_icon_scale, (m_CPIcon.texture.height / m_CPIcon.dims.y) * cp_icon_scale),
				m_CPIcon.getUVs(uv), m_CPIcon.texture.id, 50.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		}
		break;
	case MENU:
		m_resumeButton.draw(m_GUISpritebatch, m_GUICamera, true);
		m_optionsButton.draw(m_GUISpritebatch, m_GUICamera, true);
		m_quitButton.draw(m_GUISpritebatch, m_GUICamera, true);
		break;
	case OPTIONSmain:
		m_GUISpritefont.draw(m_GUISpritebatch, "OPTIONS SCREEN", glm::vec2(m_parentWindow->getScreenWidth() / 2, m_parentWindow->getScreenHeight() / 2), glm::vec2(0.25f), ALWAYS_ON_TOP, Sakura::ColorRGBA8(255, 255, 255, 255), Sakura::Justification::MIDDLE);
		break;
	default:
		std::printf("Gui has no state!\n");
		break;
	}
	std::string scoreDisplay = "Score: " + std::to_string(m_score);
	m_GUISpritefont.draw(m_GUISpritebatch, scoreDisplay.c_str(), glm::vec2(0.0f, m_GUISpritefont.getFontHeight()), glm::vec2(0.25f), ALWAYS_ON_TOP, Sakura::ColorRGBA8(255, 255, 255, 255), Sakura::Justification::LEFT);
}

void MainGUI::initComponents(){
	m_debugRenderer.init();
	m_GUISpritefont.initTTF("Assets/Fonts/destructobeambb_reg.ttf", 96, MIPMAP | LINEAR | TRANS_BORDER);
	m_shipIconTextures = m_resourceManager->getTileSheet("Assets/Sprites/UI/ship_icons.png", glm::ivec2(10, 2), MIPMAP | PIXELATED | EDGE_CLAMP);
	m_CPIcon = m_resourceManager->getTileSheet("Assets/Sprites/UI/cp_icon.png", glm::ivec2(3, 1), MIPMAP | PIXELATED | EDGE_CLAMP);
}

void MainGUI::initShipIcons(Sakura::Window* window){
	for (int i = 0; i < 10; ++i){
		m_shipIcons[i].rect.initialize(i * 48.0f * icon_scale + 10.0f, window->getScreenHeight() - 20.0f - (float)m_shipIconTextures.texture.height / (float)m_shipIconTextures.dims.y * icon_scale,
			(float)m_shipIconTextures.texture.width / (float)m_shipIconTextures.dims.x * icon_scale, (float)m_shipIconTextures.texture.height / (float)m_shipIconTextures.dims.y * icon_scale, true);
		m_shipIcons[i].shipType = (ShipType)i;
		m_shipIcons[i].name = Ship::getShipName(m_shipIcons[i].shipType);
		m_shipIcons[i].cost = Ship::getTypeCost(m_shipIcons[i].shipType);
	}
	m_shipSelector = m_resourceManager->getTileSheet("Assets/Sprites/UI/ship_selector.png", glm::ivec2(2, 1), MIPMAP | PIXELATED | EDGE_CLAMP);
}

#define BUTTON_SCALE 2.0f
void MainGUI::initButtons(Sakura::Window* window){
	m_parentWindow = window;
	m_resumeButton = createButton("Assets/Sprites/UI/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 96, glm::vec2(0.25f), MIPMAP | LINEAR | TRANS_BORDER, "Resume", glm::vec4(window->getScreenWidth() / 2 - (46 * BUTTON_SCALE), ((float)window->getScreenHeight() / 3) * 2, 92 * BUTTON_SCALE, 25 * BUTTON_SCALE),
		[this](){ state = GAMEPLAY; }, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_optionsButton = createButton("Assets/Sprites/UI/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 96, glm::vec2(0.25f), MIPMAP | LINEAR | TRANS_BORDER, "Options", glm::vec4(window->getScreenWidth() / 2 - (46 * BUTTON_SCALE), window->getScreenHeight() / 2, 92 * BUTTON_SCALE, 25 * BUTTON_SCALE),
		[this](){ state = OPTIONSmain; }, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_quitButton = createButton("Assets/Sprites/UI/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 96, glm::vec2(0.25f), MIPMAP | LINEAR | TRANS_BORDER, "Quit", glm::vec4(window->getScreenWidth() / 2 - (46 * BUTTON_SCALE), window->getScreenHeight() / 3, 92 * BUTTON_SCALE, 25 * BUTTON_SCALE),
		[this](){ m_parentScreen->setState(Sakura::ScreenState::EXIT_APPLICATION); },
		MIPMAP | PIXELATED | EDGE_CLAMP);
	
}