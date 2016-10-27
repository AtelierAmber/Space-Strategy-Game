#include "MainMenu.h"
#include <Sakura/IMainSystem.h>

#define screen_scale_level 1.0f

MainMenu::MainMenu(Sakura::Window* window) : m_window(window){
	/* Empty */
}

MainMenu::~MainMenu(){
	/* Empty */
}

int MainMenu::getNextScreenIndex() const {
	return 1;
}

int MainMenu::getPreviousScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

void MainMenu::build(){
	
}

void MainMenu::destroy(){
	
}

void MainMenu::onEntry(){
	m_GLSLSamplerName = "mySampler";
	m_GLSLPUniformName = "P";
	m_spriteBatch.init();
	m_debugRenderer.init();
	m_resourceManager.init();
	m_glyphSortType = Sakura::GlyphSortType::FRONT_TO_BACK;

	// Compile the color shader
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	// Background
	m_background = m_resourceManager.getTexture("Assets/Sprites/UI/Grid.png", MIPMAP | PIXELATED | EDGE_CLAMP);
	m_font.initTTF("Assets/Fonts/destructobeambb_reg.ttf", 144, MIPMAP | LINEAR | TRANS_BORDER);

	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(screen_scale_level);
	glm::vec2 cameraPositionOffset((m_window->getScreenWidth() / 2.0f) * (1.0f / screen_scale_level),
		(m_window->getScreenHeight() / 2.0f) * (1.0f / screen_scale_level));
	m_camera.setPosition(cameraPositionOffset);
	m_interface.init(this, &m_textureProgram, &m_resourceManager, glm::ivec2(m_window->getScreenWidth(), m_window->getScreenHeight()), cameraPositionOffset, screen_scale_level);
	m_interface.initButtons(m_window);
}

void MainMenu::onExit(){
	m_textureProgram.dispose();
	m_debugRenderer.dispose();
	m_resourceManager.destroyResources();
	m_spriteBatch.dispose();
	m_interface.destroy();
	m_font.dispose();
}

void MainMenu::update(float deltaTime){
	m_deltaTime = deltaTime;
	glm::vec2 mouseCoords = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords());
	m_interface.update(m_game->inputManager);

	m_camera.update();
	checkInput();
}

void MainMenu::checkInput(){
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
}

void MainMenu::draw(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_spriteBatch.draw(glm::vec4(0.0f, 0.0f, m_window->getScreenWidth(), m_window->getScreenHeight()), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_background.id, -500.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
	m_font.draw(m_spriteBatch, "Celestia Defense", glm::vec2((float)m_window->getScreenWidth() / 2.0f, (float)m_window->getScreenHeight() / 2.0f + 50.0f), glm::vec2(0.5f), 0.0f, Sakura::ColorRGBA8(255,255,255,255), Sakura::Justification::MIDDLE);
}

void MainMenu::specificDraw(){
	m_interface.draw(m_GLSLPUniformName, m_game->getFps());
}

//////////////////////////////////////////////////////////////////////////

void MenuGUI::destroyComponents(){
	m_startGame.destroy();
	m_quitButton.destroy();
}

void MenuGUI::update(Sakura::InputManager& inputManager){
	m_GUICamera.update();
	m_startGame.update(inputManager, m_GUICamera);
	m_quitButton.update(inputManager, m_GUICamera);
}

void MenuGUI::IDraw(float fps){
	m_startGame.draw(m_GUISpritebatch, m_GUICamera, true);
	m_quitButton.draw(m_GUISpritebatch, m_GUICamera, true);
}

void MenuGUI::initComponents(){

}

void MenuGUI::initButtons(Sakura::Window* window){
	m_startGame = createButton("Assets/Sprites/UI/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 96, glm::vec2(0.25f), MIPMAP | LINEAR | TRANS_BORDER, "Start Game", glm::vec4(window->getScreenWidth() / 2 - (46 * 2.0f), window->getScreenHeight() / 2, 92 * 2.0f, 25 * 2.0f),
		[this](){ m_parentScreen->setState(Sakura::ScreenState::CHANGE_NEXT); }, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_quitButton = createButton("Assets/Sprites/UI/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 96, glm::vec2(0.25f), MIPMAP | LINEAR | TRANS_BORDER, "Quit", glm::vec4(window->getScreenWidth() / 2 - (46 * 2.0f), window->getScreenHeight() / 3, 92 * 2.0f, 25 * 2.0f),
		[this](){ m_parentScreen->setState(Sakura::ScreenState::EXIT_APPLICATION); },
		MIPMAP | PIXELATED | EDGE_CLAMP);
}
