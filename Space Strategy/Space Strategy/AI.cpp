#include "AI.h"
#include "Grid.h"

#include <algorithm>

AI::AI(Fleet* playerFleet, std::string team, MainGUI* gui){

}

AI::~AI(){

}

void AI::init(Fleet* playerFleet, std::string team, MainGUI* gui){
	m_playerFleet = playerFleet;
	m_fleet.init(m_playerFleet, team, gui, true);
}

void AI::draw(Sakura::SpriteBatch& spriteBatch, Grid* grid, glm::vec2 mouseCoords){
	m_fleet.draw(spriteBatch, grid, mouseCoords);
}

void AI::drawDebug(Sakura::DebugRenderer& debugRenderer){
	m_fleet.drawDebug(debugRenderer);
}

bool AI::update(float deltaTime, Grid* grid){
	/* AI Calculations */
	for (auto& ship : m_fleet.getShips()){
		for (auto& pShip : m_playerFleet->getShips()){

		}
	}
	/*******************/
	return m_fleet.update(deltaTime, grid);
}

#define max_ships 20

void AI::loadNextWave(Grid* grid, Sakura::ResourceManager& resourceManager){
	++m_currentWave;
	int numShipsToSpawn = (15.0*std::atan(0.1*(double)m_currentWave) > max_ships) ? max_ships : (int)std::round(15.0*std::atan(0.1*(double)m_currentWave));
	//currentlySpawning  S_FIGHTER;
	unsigned int currentlySpawning = LEVEL1;
	if (numShipsToSpawn >= LEVEL2SPAWN){
		currentlySpawning |= LEVEL2;
		//currentlySpawning  S_BOMBER;
	}
	if (numShipsToSpawn >= LEVEL3SPAWN){
		currentlySpawning |= LEVEL3;
		//currentlySpawning  S_INTERCEPTOR;
		//currentlySpawning  S_CUTTER;
	}
	if (numShipsToSpawn >= LEVEL4SPAWN){
		currentlySpawning |= LEVEL4;
		//currentlySpawning  S_CORVETTE;
		//currentlySpawning  S_CRUISER;
	}
	if (numShipsToSpawn >= LEVEL5SPAWN){
		currentlySpawning |= LEVEL5;
		//currentlySpawning  S_CARRIER;
		//currentlySpawning  S_DESTROYER;
	}
	if (numShipsToSpawn >= LEVEL6SPAWN){
		currentlySpawning |= LEVEL6;
		/* 1 of each @ 15, 2 of each @ 20 */
		//currentlySpawning  S_ASSAULT_CARRIER;
		//currentlySpawning  S_BATTLESHIP;
	}
	/* Spawn ships */
	/* Ship creation MACRO*/
	std::vector<int> yPositions;
	for (int i = 0; i < grid->getDims().y; ++i){
		yPositions.push_back(i);
	}
#define ADD_SHIP(s_type) if(yPositions.size() > 0){\
	spawnLocation = glm::ivec2(grid->getDims().x-1, yPositions.back());\
	while (m_fleet.addShip(grid, resourceManager, s_type, glm::vec2(grid->getScreenPos(spawnLocation) + (grid->getTileDims() / 2.0f)), spawnLocation, 0, false) == -1){\
		yPositions.pop_back();\
		if(yPositions.size() <= 0) break;\
		spawnLocation = glm::ivec2(grid->getDims().x-1, yPositions.back());\
					}\
	if(yPositions.size() > 0) yPositions.pop_back();\
	if(m_fleet.getFleetSize() > 0) m_fleet.getShips().back().get()->scaleStrength(((float)m_currentWave / (float)LEVEL6SPAWN), ((float)m_currentWave / (float)LEVEL6SPAWN) / 2.0f); }
	/***************/
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(std::begin(yPositions), std::end(yPositions), gen);
	glm::ivec2 spawnLocation = glm::ivec2();
	int curMaxNumShips = numShipsToSpawn;
	if (currentlySpawning & LEVEL6){
		std::uniform_int_distribution<int> randL7Type(0, 1);
		while (numShipsToSpawn >= curMaxNumShips - (int)((numShipsToSpawn - LEVEL6SPAWN) / 2)){
			if (randL7Type(gen) == 0){
				//Spawn BattleShip
				ADD_SHIP(ShipType::BATTLESHIP);
			}
			else {
				//Spawn Assault Carrier
				ADD_SHIP(ShipType::ASSAULT_CARRIER);
			}
			--numShipsToSpawn;
		}
	}
	curMaxNumShips = numShipsToSpawn;
	if (currentlySpawning & LEVEL5){
		std::uniform_int_distribution<int> randL6Type(0, 2);
		while (numShipsToSpawn > curMaxNumShips - 2){
			if (randL6Type(gen) == 0){
				//Spawn Carrier
				ADD_SHIP(ShipType::CARRIER);
			}
			else {
				//Spawn Destroyer
				ADD_SHIP(ShipType::DESTROYER);
			}
			--numShipsToSpawn;
		}
	}
	curMaxNumShips = numShipsToSpawn;
	if (currentlySpawning & LEVEL4){
		while (numShipsToSpawn > curMaxNumShips - ((currentlySpawning & LEVEL5SPAWN) ? 2 : 1)){
			//Spawn Corvette
			ADD_SHIP(ShipType::CORVETTE);
			--numShipsToSpawn;
		}
	}
	curMaxNumShips = numShipsToSpawn;
	if (currentlySpawning & LEVEL3){
		std::uniform_int_distribution<int> randL4Type(1, 5);
		while (numShipsToSpawn > curMaxNumShips - (int)std::round((double)numShipsToSpawn/3.0)){
			if (randL4Type(gen) == 2 || randL4Type(gen) == 4){
				//Spawn Interceptor
				ADD_SHIP(ShipType::INTERCEPTOR);
			}
			else {
				//Spawn Cutter
				ADD_SHIP(ShipType::CUTTER);
			}
			--numShipsToSpawn;
		}
	}
	curMaxNumShips = numShipsToSpawn;
	if (currentlySpawning & LEVEL2){
		while (numShipsToSpawn > curMaxNumShips - ((currentlySpawning & LEVEL4SPAWN) ? ((currentlySpawning & LEVEL5SPAWN) ? 3 : 2) : 1)){
			//Spawn Bomber
			ADD_SHIP(ShipType::BOMBER);	
			--numShipsToSpawn;
		}
	}
	while (numShipsToSpawn && (int)yPositions.size() > 0){
		//Spawn "Fodder"
		ADD_SHIP((currentlySpawning & LEVEL4) ? ShipType::CRUISER : ShipType::FIGHTER);
		--numShipsToSpawn;
	}
	yPositions.clear();
}
