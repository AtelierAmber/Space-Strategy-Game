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
	m_screen = std::make_unique<MainScreen>(&m_window);

	m_screenList->addScreen(m_screen.get());
	m_screenList->setScreen(m_screen->getScreenIndex());
}

void BaseGame::onExit(){

}
