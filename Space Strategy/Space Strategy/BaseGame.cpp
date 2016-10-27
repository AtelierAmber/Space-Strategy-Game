#include "BaseGame.h"
#include <Sakura/ScreenList.h>

BaseGame::BaseGame(std::string title /*= "DEFAULT TITLE"*/, unsigned int screenWidth /*= 500*/, unsigned int screenHeight /*= 500*/, unsigned int windowFlags /*= 0*/, int winPositionX /*= -1*/, int winPositionY /*= -1*/) : 
	IMainSystem(title.c_str(), screenWidth, screenHeight, windowFlags, winPositionX, winPositionY){

}

BaseGame::~BaseGame(){

}

void BaseGame::onInit(){

}

void BaseGame::addScreens(){
	m_mainMenu = std::make_unique<MainMenu>(&m_window);
	m_screen = std::make_unique<MainScreen>(&m_window, &m_playerScore);
	m_gameOverMenu = std::make_unique<GameOver>(&m_window, &m_playerScore);

	m_screenList->addScreen(m_mainMenu.get());
	m_screenList->addScreen(m_screen.get());
	m_screenList->addScreen(m_gameOverMenu.get());
	m_screenList->setScreen(m_gameOverMenu->getScreenIndex());
}

void BaseGame::onExit(){

}
