#pragma once

#include <Sakura/GUIInterface.h>
#include <Sakura/Window.h>

#include "Ship.h"

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

#define icon_scale 1.5f

class Fleet;

class MainGUI :
	public Sakura::GUIInterface
{
public:

	MainGUI();
	~MainGUI();

	void destroyComponents() override;

	void update(Sakura::InputManager& inputManager) override;
	bool updateIcons(Sakura::InputManager& inputManager, const glm::vec2& mouseCoords, Fleet* pFleet, bool& placingShips);

	void IDraw(float fps) override;

	void initComponents() override;

	void initShipIcons(Sakura::Window* window);
	void initWaves(int* wavesRef);
	void initButtons(Sakura::Window* window);
	void initCommandship(Ship* commandship){ m_commandship = commandship; }

	void setState(GUIState newState){ state = newState; }
	GUIState getState(){ return state; }

	void setSelectedShipType(ShipType type){ m_selectedShipType = type; }

	CP getMaxCP(){ return m_CP; }
	CP getUsedCP(){ return m_usedCP; }
	void addUsedCP(CP cp){ m_usedCP += cp; }

	void addScore(int score){ m_score += score; }
	int* getScore(){ return &m_score; }

private:
	/* Resume, Options, Quit */
	Sakura::GUIButton m_resumeButton;
	Sakura::GUIButton m_optionsButton;
	Sakura::GUIButton m_quitButton;

	/* Fleet Managment */
	Sakura::GUIButton m_addShipsButton;
	Sakura::TileSheet m_shipIconTextures;
	ShipIcon m_shipIcons[10];
	Sakura::TileSheet m_shipSelector;
	ShipType m_selectedShipType = ShipType::CUTTER;

	Sakura::TileSheet m_CPIcon;
	Sakura::TileSheet m_hearts;
	CP m_CP = 50;
	CP m_usedCP = 0;

	Ship* m_commandship;
	int* m_currentWave = nullptr;
	int m_score;

	Sakura::Window* m_parentWindow;
	GUIState state = GAMEPLAY;
	Sakura::DebugRenderer m_debugRenderer;
};