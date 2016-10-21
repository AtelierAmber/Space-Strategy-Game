#pragma once

#include <Sakura/IScreen.h>
#include <Sakura/GUIInterface.h>
#include <Sakura/Window.h>
#include <Sakura/SpriteBatch.h>
#include <Sakura/SpriteFont.h>
#include <Sakura/DebugRenderer.h>
#include <Sakura/GLSLProgram.h>
#include <Sakura/Camera2D.h>
#include <Sakura/ResourceManager.h>
#include "Grid.h"

enum GUIState{
	GAMEPLAY = 0,
	MENU = 1,
	OPTIONSmain = 2,
	FLEET = 3
};

class MainGUI :
	public Sakura::GUIInterface
{
public:
	
	MainGUI();
	~MainGUI();

	void destroyComponents() override;

	void update(Sakura::InputManager& inputManager) override;

	void IDraw(float fps) override;

	void initComponents() override;

	void initButtons(Sakura::Window* window);

	void setState(GUIState newState){ state = newState; }
	GUIState getState(){ return state; }

private:
	/* Resume, Options, Quit */
	Sakura::GUIButton m_resumeButton;
	Sakura::GUIButton m_optionsButton;
	Sakura::GUIButton m_quitButton;

	/* Fleet Managment */
	Sakura::GUIButton m_warshipsButton;

	Sakura::Window* m_parentWindow;
	GUIState state = GAMEPLAY;
};

class Ship;

class MainScreen :
	public Sakura::IScreen
{
public:
	MainScreen(Sakura::Window* window);
	~MainScreen();

	//Returns the index of the next or previous screen when changing screens
	int getNextScreenIndex() const override;
	int getPreviousScreenIndex() const override;

	//Called at beginning and end of application
	void build() override;
	void destroy() override;

	//Called when a screen enters and exits focus
	void onEntry() override;
	void onExit() override;

	//Called in the main game loop
	void update(float deltaTime) override;
	void checkInput();
	void draw() override;
	void specificDraw() override;

	//HACK DEBUG ELEMENTS | REMOVE BEFORE RELEASE
	void drawDebugElements();
	//////////////////////////////////////////////////////////////////////////
private:
	//HACK DEBUG ELEMENTS | REMOVE BEFORE RELEASE
	bool debug_game_events = true;
	bool show_boxes = true;
	Sakura::SpriteFont m_debugFont;
	Sakura::Rect m_debugRects[1];
	//////////////////////////////////////////////////////////////////////////

	float m_deltaTime = 1.0f;

	Sakura::Window* m_window = nullptr;

	Sakura::GLTexture m_background;

	MainGUI m_interface;

	Grid m_grid;
	std::vector<Ship> m_ships;

	Sakura::Music m_mainMusic;
	Sakura::Music m_bossMusic;
	glm::vec2 m_previousMouseLocation;
	Sakura::SpriteFont m_userFont;
	Sakura::SpriteFont m_enemyFont;
};