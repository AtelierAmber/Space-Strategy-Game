#include "GameOver.h"
#include <Sakura/IMainSystem.h>

#define screen_scale_level 1.0f

GameOver::GameOver(Sakura::Window* window, int* score) : m_window(window), m_finalScore(score){
	/* Empty */
}

GameOver::~GameOver(){
	/* Empty */
}

int GameOver::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int GameOver::getPreviousScreenIndex() const {
	return 1;
}

void GameOver::build(){

}

void GameOver::destroy(){

}

void GameOver::onEntry(){
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
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(screen_scale_level);
	glm::vec2 cameraPositionOffset((m_window->getScreenWidth() / 2.0f) * (1.0f / screen_scale_level),
		(m_window->getScreenHeight() / 2.0f) * (1.0f / screen_scale_level));
	m_camera.setPosition(cameraPositionOffset);
	m_interface.init(this, &m_textureProgram, &m_resourceManager, glm::ivec2(m_window->getScreenWidth(), m_window->getScreenHeight()), cameraPositionOffset, screen_scale_level);
}

void GameOver::onExit(){
	m_textureProgram.dispose();
	m_debugRenderer.dispose();
	m_resourceManager.destroyResources();
	m_spriteBatch.dispose();
	m_interface.destroy();
}

void GameOver::update(float deltaTime){

}

void GameOver::checkInput(){

}

void GameOver::draw(){

}

void GameOver::specificDraw(){

}

//////////////////////////////////////////////////////////////////////////

void GameOverGUI::destroyComponents(){

}

void GameOverGUI::update(Sakura::InputManager& inputManager){

}

void GameOverGUI::IDraw(float fps){

}

void GameOverGUI::initComponents(){
	m_GUISpritefont.initTTF("Assets/Fonts/destructobeambb_reg.ttf", 96, MIPMAP | LINEAR | TRANS_BORDER);

}
