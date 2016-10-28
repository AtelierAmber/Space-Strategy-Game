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
	return 0;
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
	m_blip = m_resourceManager.loadSoundEffect("Assets/Audio/SFX/select.wav");

	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(screen_scale_level);
	glm::vec2 cameraPositionOffset((m_window->getScreenWidth() / 2.0f) * (1.0f / screen_scale_level),
		(m_window->getScreenHeight() / 2.0f) * (1.0f / screen_scale_level));
	m_camera.setPosition(cameraPositionOffset);
	m_saveScore.init("Assets/Sprites/UI/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 96, glm::vec2(0.25f), MIPMAP | LINEAR | TRANS_BORDER, "Save Score", glm::vec4((float)m_window->getScreenWidth() / 3.0f - (46.0f * 2.0f), (float)m_window->getScreenHeight() / 3.0f, 92.0f * 2.0f, 25.0f * 2.0f),
		[this](){ m_displayState = SAVE_NAME; }, m_resourceManager, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_retryButton.init("Assets/Sprites/UI/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 96, glm::vec2(0.25f), MIPMAP | LINEAR | TRANS_BORDER, "Retry", glm::vec4(((float)m_window->getScreenWidth() / 3.0f) * 2.0f - (46.0f * 2.0f), (float)m_window->getScreenHeight() / 3.0f, 92.0f * 2.0f, 25.0f * 2.0f),
		[this](){ setState( Sakura::ScreenState::CHANGE_PREVIOUS); }, m_resourceManager, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_quitButton.init("Assets/Sprites/UI/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 96, glm::vec2(0.25f), MIPMAP | LINEAR | TRANS_BORDER, "Quit", glm::vec4((float)m_window->getScreenWidth() / 2.0f - (46.0f * 2.0f), (float)m_window->getScreenHeight() / 5.0f, 92.0f * 2.0f, 25.0f * 2.0f),
		[this](){ setState(Sakura::ScreenState::EXIT_APPLICATION); }, m_resourceManager, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_continueButton.init("Assets/Sprites/UI/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 96, glm::vec2(0.25f), MIPMAP | LINEAR | TRANS_BORDER, " -> ", glm::vec4((float)m_window->getScreenWidth() / 2.0f + (100.0f), (float)m_window->getScreenHeight() / 2.0f , 46.0f , 25.0f ),
		[this](){ saveScore(); setState(Sakura::ScreenState::CHANGE_NEXT); }, m_resourceManager, MIPMAP | PIXELATED | EDGE_CLAMP);

	m_font.initTTF("Assets/Fonts/destructobeambb_reg.ttf", 96, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_currentScore.score = *m_finalScore;
	m_chosenNameChar[0] = " A ";
	m_chosenNameChar[1] = " A ";
	m_chosenNameChar[2] = " A ";
	m_scoreFile.open("Scores/scores.goml", std::ios::in | std::ios::binary);
	if (!m_scoreFile.fail()){
		ScoreStruct tmpScore;
		while (m_scoreFile.read((char*)&tmpScore, sizeof(ScoreStruct))){
			m_scores.push_back(tmpScore);
		}
	}
	else std::printf("Opening scores file failed!!!");
	for (std::size_t i = 0; i < m_scores.size(); ++i){
		std::printf("%i : %c%c%c - %i", m_scores[i].ranking, m_scores[i].name[0], m_scores[i].name[1], m_scores[i].name[2], m_scores[i].score);
	}
	m_scoreFile.close();
}

void GameOver::onExit(){
	m_textureProgram.dispose();
	m_debugRenderer.dispose();
	m_resourceManager.destroyResources();
	m_spriteBatch.dispose();
	m_saveScore.destroy();
	m_retryButton.destroy();
	m_quitButton.destroy();
	m_continueButton.destroy();
	m_scores.clear();
	m_font.dispose();
}

void GameOver::update(float deltaTime){
	glm::vec2 mouseCoords = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords());
	switch (m_displayState){
	case MAIN:
		m_saveScore.update(m_game->inputManager, m_camera);
		m_retryButton.update(m_game->inputManager, m_camera);
		m_quitButton.update(m_game->inputManager, m_camera);
		break;
	case SAVE_NAME:
		m_quitButton.update(m_game->inputManager, m_camera);
		m_continueButton.update(m_game->inputManager, m_camera);
		m_chosenNameChar[m_nameCharIndex][1] = (m_letterIndex + 17) + '0';
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
		m_quitButton.draw(m_spriteBatch, m_camera, true);
		break;
	case SAVE_NAME:
	{
		m_quitButton.draw(m_spriteBatch, m_camera, true);
		m_continueButton.draw(m_spriteBatch, m_camera, true);

		for (int i = 0; i < 3; ++i){
			float scaleSelec = (i == m_nameCharIndex) ? 1.5f : 1.0f;
			m_spriteBatch.draw(glm::vec4((float)m_window->getScreenWidth() / 2.0f - m_nameArrow.width + (50.0f * (i - 1)), (float)m_window->getScreenHeight() / 2.0f + 50.0f, m_nameArrow.width*2.0f, m_nameArrow.height*2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_nameArrow.id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
			m_spriteBatch.draw(glm::vec4((float)m_window->getScreenWidth() / 2.0f - m_nameArrow.width + (50.0f * (i - 1)), (float)m_window->getScreenHeight() / 2.0f - 50.0f, m_nameArrow.width*2.0f, m_nameArrow.height*2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_nameArrow.id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255), 3.141592f);
			m_debugRenderer.drawBox(glm::vec4(m_window->getScreenWidth() / 2.0f - m_nameArrow.width + (50.0f * (i - 1)), (float)m_window->getScreenHeight() / 2 + m_nameArrow.height*2.0f - 40.0f, m_nameArrow.width*2.0f, 65.0f), Sakura::ColorRGBA8(255, 255, 255, 255), 0.0f);
			std::string nameChar = m_chosenNameChar[i];
			m_font.draw(m_spriteBatch, nameChar.c_str(), glm::vec2((float)m_window->getScreenWidth() / 2.0f - m_nameArrow.width + (50.0f * (i - 1)) - ((scaleSelec - 1.0f) * 10.0f), (float)m_window->getScreenHeight() / 2), glm::vec2(0.2f * scaleSelec), 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		}
		break;
	}
	default:
		break;
	}
	std::string finalScore = "Final Score: " + std::to_string((*m_finalScore));
	m_font.draw(m_spriteBatch, finalScore.c_str(), glm::vec2((float)m_window->getScreenWidth() / 2.0f, (float)m_window->getScreenHeight() / 2 + 150.0f), glm::vec2(0.25f), 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255), Sakura::Justification::MIDDLE);
}

void GameOver::specificDraw(){

}

void GameOver::sortScores(int left, int right){
	/* Sort fleet using quick sort
	* Can't use std::sort or std::qsort due to changing the id of the ships */
	if (left >= right) {
		return;
	}

	int part = partition(left, right);

	sortScores(left, part - 1);
	sortScores(part + 1, right);
}

int GameOver::partition(int left, int right){
	int mid = left + (right - left) / 2;
	int pivot = m_scores[mid].score;
	// Move the mid point value to the front.
	std::swap(m_scores[mid], m_scores[left]);
	m_scores[mid].ranking = mid+1;
	m_scores[left].ranking = left+1;
	int i = left + 1;
	int j = right;

	while (i <= j) {
		while (i <= j && m_scores[i].score <= pivot) {
			++i;
		}
		while (i <= j && m_scores[j].score > pivot) {
			--j;
		}
		if (i < j) {
			std::swap(m_scores[i], m_scores[j]);
			m_scores[j].ranking = j+1;
			m_scores[i].ranking = i+1;
		}
	};
	std::swap(m_scores[i - 1], m_scores[left]);
	m_scores[i - 1].ranking = (i);
	m_scores[left].ranking = left+1;
	return i - 1;
}

void GameOver::saveScore(){
	m_currentScore.name[0] = m_chosenNameChar[0][1];
	m_currentScore.name[1] = m_chosenNameChar[1][1];
	m_currentScore.name[2] = m_chosenNameChar[2][1];
	m_scores.push_back(m_currentScore);
	m_scores.back().ranking = m_scores.size();
	sortScores(0, m_scores.size() - 1);
	std::reverse(m_scores.begin(), m_scores.end());
	for (std::size_t i = 0; i < m_scores.size(); ++i){
		m_scores[i].ranking = i + 1;
	}
	m_scoreFile.open("Scores/scores.goml", std::ios::binary | std::ios::trunc | std::ios::out);
	if (!m_scoreFile.fail()){
		m_scoreFile.seekg(0);
		for (std::size_t i = 0; i < m_scores.size(); ++i){
			m_scoreFile.write((char*)&m_scores[i], sizeof(ScoreStruct));
			std::printf("%i : %c%c%c - %i", m_scores[i].ranking, m_scores[i].name[0], m_scores[i].name[1], m_scores[i].name[2], m_scores[i].score);
		}
	}
	m_scoreFile.close();
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
		m_blip.play();
	}
	if (m_game->inputManager.isKeyPressed(KeyID::DOWN)){
		--m_letterIndex;
		if (m_letterIndex < 0){
			m_letterIndex = 25;
		}
		m_blip.play();
	}
	if (m_game->inputManager.isKeyPressed(KeyID::LEFT)){
		if (m_nameCharIndex > 0) --m_nameCharIndex;
		m_letterIndex = (m_chosenNameChar[m_nameCharIndex][1] - '0' - 17);
		m_blip.play();
	}
	if (m_game->inputManager.isKeyPressed(KeyID::RIGHT)){
		if (m_nameCharIndex < 2) ++m_nameCharIndex;
		m_letterIndex = (m_chosenNameChar[m_nameCharIndex][1] - '0' - 17);
		m_blip.play();
	}

}