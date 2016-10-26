#pragma once

#include "Fleet.h"

enum ShipSpawns{
	LEVEL1 = 0x1,
	LEVEL2 = 0x2,
	LEVEL3 = 0x4,
	LEVEL4 = 0x8,
	LEVEL5 = 0x10,
	LEVEL6 = 0x20,
};

#define LEVEL2SPAWN 2
#define LEVEL3SPAWN 4
#define LEVEL4SPAWN 7
#define LEVEL5SPAWN 10
#define LEVEL6SPAWN 15

class AI
{
public:
	AI(Fleet* playerFleet, std::string team, MainGUI* gui);
	AI(){ /* Emtpy */ }
	~AI();

	void init(Fleet* playerFleet, std::string team, MainGUI* gui);

	const Fleet& getFleet() const{ return m_fleet; }
	Ship* enemyShipAtPosition(glm::vec2 position){ return m_fleet.shipAtPosition(position); }

	void draw(Sakura::SpriteBatch& spriteBatch, Grid* grid, glm::vec2 mouseCoords);
	void drawDebug(Sakura::DebugRenderer& debugRenderer);

	bool update(float deltaTime, Grid* grid);

	void loadNextWave(Grid* grid, Sakura::ResourceManager& resourceManager);

	Fleet* FleetPtr() { return &m_fleet; }
private:
	Fleet m_fleet;
	int m_currentWave = 15;
	Fleet* m_playerFleet = nullptr;
};
