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
#include "Fleet.h"

enum GUIState{
	GAMEPLAY = 0,
	MENU = 1,
	OPTIONSmain = 2,
	FLEET = 3
};

struct ShipIcon {
	Sakura::Rect rect;
	bool unlocked = false;
	bool hover = false;
	std::string name = "";
	int cost = 0;
	ShipType shipType = ShipType::NOSHIP;
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

	void initShipIcons(Sakura::Window* window);
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
	Sakura::TileSheet m_shipIconTextures;
	ShipIcon m_shipIcons[10];

	Sakura::Window* m_parentWindow;
	GUIState state = GAMEPLAY;
	Sakura::DebugRenderer m_debugRenderer;
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
	Fleet m_fleet;
	Ship* m_selectedShip = nullptr;

	Sakura::Music m_mainMusic;
	Sakura::Music m_bossMusic;
	glm::vec2 m_previousMouseLocation;
	Sakura::SpriteFont m_userFont;
	Sakura::SpriteFont m_enemyFont;
};