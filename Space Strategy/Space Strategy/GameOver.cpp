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

	m_nameArrow = m_resourceManager.getTexture("Assets/Sprites/UI/name_choose_arrow.png", MIPMAP | PIXELATED | EDGE_CLAMP);

	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(screen_scale_level);
	glm::vec2 cameraPositionOffset((m_window->getScreenWidth() / 2.0f) * (1.0f / screen_scale_level),
		(m_window->getScreenHeight() / 2.0f) * (1.0f / screen_scale_level));
	m_camera.setPosition(cameraPositionOffset);
	m_saveScore.init("Assets/Sprites/UI/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 96, glm::vec2(0.25f), MIPMAP | LINEAR | TRANS_BORDER, "Save Score", glm::vec4((float)m_window->getScreenWidth() / 3.0f - (46.0f * 2.0f), (float)m_window->getScreenHeight() / 3.0f, 92.0f * 2.0f, 25.0f * 2.0f),
		[this](){ m_displayState = SAVE_NAME; }, m_resourceManager, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_retryButton.init("Assets/Sprites/UI/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 96, glm::vec2(0.25f), MIPMAP | LINEAR | TRANS_BORDER, "Retry", glm::vec4(((float)m_window->getScreenWidth() / 3.0f) * 2.0f - (46.0f * 2.0f), (float)m_window->getScreenHeight() / 3.0f, 92.0f * 2.0f, 25.0f * 2.0f),
		[this](){ setState( Sakura::ScreenState::CHANGE_PREVIOUS); }, m_resourceManager, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_quitButton.init("Assets/Sprites/UI/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 96, glm::vec2(0.25f), MIPMAP | LINEAR | TRANS_BORDER, "Quit", glm::vec4((float)m_window->getScreenWidth() / 3.0f - (46.0f * 2.0f), (float)m_window->getScreenHeight() / 3.0f, 92.0f * 2.0f, 25.0f * 2.0f),
		[this](){ setState(Sakura::ScreenState::EXIT_APPLICATION); }, m_resourceManager, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_font.initTTF("Assets/Fonts/destructobeambb_reg.ttf", 96, MIPMAP | PIXELATED | EDGE_CLAMP);
}

void GameOver::onExit(){
	m_textureProgram.dispose();
	m_debugRenderer.dispose();
	m_resourceManager.destroyResources();
	m_spriteBatch.dispose();
	m_saveScore.destroy();
	m_retryButton.destroy();
	m_quitButton.destroy();
	m_scores.clear();
	m_font.dispose();
}

void GameOver::update(float deltaTime){
	glm::vec2 mouseCoords = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords());

	switch (m_displayState){
	case MAIN:
		m_saveScore.update(m_game->inputManager, m_camera);
		m_retryButton.update(m_game->inputManager, m_camera);
		break;
	case SAVE_NAME:
		m_quitButton.update(m_game->inputManager, m_camera);
		m_retryButton.update(m_game->inputManager, m_camera);
		switch (m_nameCharIndex){
		case 0:
			m_chosenNameChar1[0] = (m_letterIndex + 17) + '0';
			break;
		case 1:
			m_chosenNameChar2[0] = (m_letterIndex + 17) + '0';
			break;
		case 2:
			m_chosenNameChar3[0] = (m_letterIndex + 17) + '0';
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	m_camera.update();
	checkInput();
}

void GameOver::draw(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	switch (m_displayState){
	case MAIN:
		m_saveScore.draw(m_spriteBatch, m_camera, true);
		m_retryButton.draw(m_spriteBatch, m_camera, true);
		break;
	case SAVE_NAME:
	{
		m_quitButton.draw(m_spriteBatch, m_camera, true);
		m_retryButton.draw(m_spriteBatch, m_camera, true);

		m_spriteBatch.draw(glm::vec4(m_window->getScreenWidth() / 2.0f - 75.0f, m_window->getScreenHeight() / 2 + 50.0f, m_nameArrow.width*2.0f, m_nameArrow.height*2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_nameArrow.id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		m_spriteBatch.draw(glm::vec4(m_window->getScreenWidth() / 2.0f - 75.0f, m_window->getScreenHeight() / 2 - 50.0f, m_nameArrow.width*2.0f, m_nameArrow.height*2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_nameArrow.id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255), 3.141592);
		m_debugRenderer.drawBox(glm::vec4(m_window->getScreenWidth() / 2.0f - 75.0f, m_window->getScreenHeight() / 2 - 40.0f, m_nameArrow.width*2.0f, 80.0f), Sakura::ColorRGBA8(255, 255, 0, 255), 0.0f);
		std::string nameChar = m_chosenNameChar1;
		m_font.draw(m_spriteBatch, nameChar.c_str(), glm::vec2(m_window->getScreenWidth() / 2.0f - 75.0f, m_window->getScreenHeight() / 2 + m_nameArrow.height*2.0f), glm::vec2(0.2f), 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255));

		m_spriteBatch.draw(glm::vec4(m_window->getScreenWidth() / 2.0f + m_nameArrow.width, m_window->getScreenHeight() / 2 + 50.0f, m_nameArrow.width*2.0f, m_nameArrow.height*2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_nameArrow.id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		m_spriteBatch.draw(glm::vec4(m_window->getScreenWidth() / 2.0f + m_nameArrow.width, m_window->getScreenHeight() / 2 - 50.0f, m_nameArrow.width*2.0f, m_nameArrow.height*2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_nameArrow.id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255), 3.141592);
		m_debugRenderer.drawBox(glm::vec4(m_window->getScreenWidth() / 2.0f + 20.0f, m_window->getScreenHeight() / 2 - 40.0f, m_nameArrow.width*2.0f, 80.0f), Sakura::ColorRGBA8(255, 0, 255, 255), 0.0f);
		nameChar = m_chosenNameChar2;
		m_font.draw(m_spriteBatch, nameChar.c_str(), glm::vec2(m_window->getScreenWidth() / 2.0f + 20.0f, m_window->getScreenHeight() / 2 + m_nameArrow.height*2.0f), glm::vec2(0.2f), 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255));

		m_spriteBatch.draw(glm::vec4(m_window->getScreenWidth() / 2.0f + 75.0f, m_window->getScreenHeight() / 2 + 50.0f, m_nameArrow.width*2.0f, m_nameArrow.height*2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_nameArrow.id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		m_spriteBatch.draw(glm::vec4(m_window->getScreenWidth() / 2.0f + 75.0f, m_window->getScreenHeight() / 2 - 50.0f, m_nameArrow.width*2.0f, m_nameArrow.height*2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_nameArrow.id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255), 3.141592);
		m_debugRenderer.drawBox(glm::vec4(m_window->getScreenWidth() / 2.0f - 50.0f - m_nameArrow.width*2.0f, m_window->getScreenHeight() / 2 - 40.0f, m_nameArrow.width*2.0f, 80.0f), Sakura::ColorRGBA8(0, 255, 255, 255), 0.0f);
		nameChar = m_chosenNameChar3;
		m_font.draw(m_spriteBatch, nameChar.c_str(), glm::vec2(m_window->getScreenWidth() / 2.0f + 75.0f, m_window->getScreenHeight() / 2 + m_nameArrow.height*2.0f), glm::vec2(0.2f), 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		break;
	}
	default:
		break;
	}
	std::string finalScore = "Final Score: " + std::to_string((*m_finalScore));
	m_font.draw(m_spriteBatch, finalScore.c_str(), glm::vec2(m_window->getScreenWidth() / 2.0f, m_window->getScreenHeight() / 2 + 150.0f), glm::vec2(0.25f), 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255), Sakura::Justification::MIDDLE);
}

void GameOver::specificDraw(){

}

void GameOver::checkInput(){
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}

	if (m_game->inputManager.isKeyPressed(KeyID::UP)){
		++m_letterIndex;
		if (m_letterIndex > 25){
			m_letterIndex = 0;
		}
	}
	if (m_game->inputManager.isKeyPressed(KeyID::DOWN)){
		--m_letterIndex;
		if (m_letterIndex < 0){
			m_letterIndex = 25;
		}
	}
	if (m_game->inputManager.isKeyPressed(KeyID::LEFT)){
		if (m_nameCharIndex > 0) --m_nameCharIndex;
	}
	if (m_game->inputManager.isKeyPressed(KeyID::RIGHT)){
		if (m_nameCharIndex < 2) ++m_nameCharIndex;
	}

}