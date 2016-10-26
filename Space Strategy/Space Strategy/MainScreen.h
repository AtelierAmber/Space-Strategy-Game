#pragma once

#include <Sakura/IScreen.h>
#include <Sakura/SpriteBatch.h>
#include <Sakura/SpriteFont.h>
#include <Sakura/DebugRenderer.h>
#include <Sakura/GLSLProgram.h>
#include <Sakura/Camera2D.h>
#include <Sakura/ResourceManager.h>

#include "Grid.h"
#include "Fleet.h"
#include "MainGUI.h"
#include "GhostShip.h"
#include "AI.h"

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

	void proceedTurn(){ m_turnsFinished = false; }

	//HACK DEBUG ELEMENTS | REMOVE BEFORE RELEASE
	void drawDebugElements();
	//////////////////////////////////////////////////////////////////////////
private:
	//HACK DEBUG ELEMENTS | REMOVE BEFORE RELEASE
	bool debug_game_events = false;
	bool show_boxes = false;
	Sakura::SpriteFont m_debugFont;
	Sakura::Rect m_debugRects[1];
	//////////////////////////////////////////////////////////////////////////

	float m_deltaTime = 1.0f;
	float m_timer = 0.0f;

	bool m_placingShips = false;
	GhostShip m_shipToPlace;
	bool m_turnsFinished = true;

	Sakura::Window* m_window = nullptr;

	Sakura::GLTexture m_background;

	MainGUI m_interface;
	Sakura::GUIButton m_addShipsButton;
	Sakura::GUIButton m_readyButton;

	Grid m_grid;
	Fleet m_playerFleet;
	AI m_ai;

	Sakura::Music m_mainMusic;
	Sakura::Music m_bossMusic;
	glm::vec2 m_previousMouseLocation;
	Sakura::SpriteFont m_userFont;
	Sakura::SpriteFont m_enemyFont;
};