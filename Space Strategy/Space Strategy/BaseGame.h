#pragma once

#include <Sakura/IMainSystem.h>
#include <Sakura/IScreen.h>

#include "MainScreen.h"
#include "MainMenu.h"
#include "GameOver.h"

class BaseGame : 
	public Sakura::IMainSystem
{
public:
	BaseGame(std::string title = "DEFAULT TITLE", unsigned int screenWidth = 500, unsigned int screenHeight = 500, unsigned int windowFlags = 0, int winPositionX = -1, int winPositionY = -1);
	~BaseGame();

	// Called on initialization
	void onInit();
	// For adding all screens
	void addScreens();
	// Called when exiting
	void onExit();
private:
	std::unique_ptr<MainScreen> m_screen = nullptr;
	std::unique_ptr<MainMenu> m_mainMenu = nullptr;
	std::unique_ptr<GameOver> m_gameOverMenu = nullptr;

	int m_playerScore;
};

