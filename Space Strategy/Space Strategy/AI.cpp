#include "AI.h"
#include "Grid.h"

#include <algorithm>

AI::AI(Fleet* playerFleet, std::string team, MainGUI* gui){

}

AI::~AI(){

}

void AI::init(Fleet* playerFleet, std::string team, MainGUI* gui, Sakura::ResourceManager* resourceManager, Grid* grid){
	m_playerFleet = playerFleet;
	m_fleet.init(m_playerFleet, team, gui, true, resourceManager, grid);
	gen = std::mt19937(rd());
}

void AI::destroy(){
	m_fleet.destroy();
	m_currentWave = 0;
}

void AI::draw(Sakura::SpriteBatch& spriteBatch, Grid* grid, glm::vec2 mouseCoords){
	m_fleet.draw(spriteBatch, grid, mouseCoords);
}

void AI::drawDebug(Sakura::DebugRenderer& debugRenderer){
	m_fleet.drawDebug(debugRenderer);
}

void AI::spawnCarrierChild(Ship* ship){
	/* Spawn a fighter at one of the ten tiles around it */
	std::vector<int> tiles;
	for (int i = 0; i < 10; ++i){
		tiles.push_back(i);
	}
	std::shuffle(std::begin(tiles), std::end(tiles), gen);
	glm::ivec2 posOff = glm::ivec2(0);
	if (tiles.back() < 4){
		posOff.y = 1;
	}
	else if (tiles.back() < 6){
		posOff.y = 0;
	}
	else posOff.y = -1;
	if (tiles.back() == 0 || tiles.back() == 4 || tiles.back() == 6){
		posOff.x = 2;
	}
	else if (tiles.back() == 1 || tiles.back() == 7){
		posOff.x = 1;
	}
	else if (tiles.back() == 2 || tiles.back() == 8){
		posOff.x = 0;
	}
	else posOff.x = -1;
	glm::ivec2 spawnLoc = ship->getPosition() - posOff;
	//Call the ship callback to spawn the fighter
	int callReturn = ship->callUnique(&spawnLoc);
	while (callReturn != 0){
		tiles.pop_back();
		if (tiles.size() <= 0) return;
		glm::ivec2 posOff = glm::ivec2(0);
		if (tiles.back() < 4){
			posOff.y = 1;
		}
		else if (tiles.back() < 6){
			posOff.y = 0;
		}
		else posOff.y = -1;
		if (tiles.back() == 0 || tiles.back() == 4 || tiles.back() == 6){
			posOff.x = 2;
		}
		else if (tiles.back() == 1 || tiles.back() == 7){
			posOff.x = 1;
		}
		else if (tiles.back() == 2 || tiles.back() == 8){
			posOff.x = 0;
		}
		else posOff.x = -1;
		spawnLoc = ship->getPosition() - posOff;
		callReturn = ship->callUnique(&spawnLoc);
	}
}

#define PI 3.141592f

bool AI::update(float deltaTime, Grid* grid){
	/* AI Calculations */
	if (!m_hasUpdatedOnce){
		for (auto& ship : m_fleet.getShips()){
			std::uniform_real_distribution<float> moveDirection(PI / 4, (7* PI)/4);
			float moveDir = moveDirection(gen);
			glm::ivec2 dirVec = glm::ivec2(ship->getSpeed()*std::cos(moveDir), ship->getSpeed()*std::sin(moveDir));
			glm::ivec2 newPos = ship->getPosition() + dirVec;
			ship->move(newPos, grid, m_playerFleet);
			
			float distanceTo = 0.0f;
			for (int i = 0; i < m_playerFleet->getFleetSize(); ++i){
				distanceTo = glm::length(glm::vec2(ship->getPosition() - ((*m_playerFleet)[i]->getPosition() + ((*m_playerFleet)[i]->getTileSpan()/2))));
				if (distanceTo < ship.get()->getAttackRange() + 1){
					ship.get()->queueAttack(m_playerFleet->getShips()[i].get());
					break;
				}
			}

			/* ADVANCED ALGORITHM > NO TIME TO IMPLEMENT > WILL GO BACK AND ADD */
// 			auto& ship = m_fleet.getShips()[i];
// 			/* Calculations
// 				 * if pShip can attack ship && pShip.shield damage > ship.shield/2 || (!ship.shield && pShip.hull damage > ship.hull/2) -> run away
// 				 * if pShip in range of ship.speed & ship.attack && pShip largest threat level -> move towards
// 				 * if pShip in range && pShip is largest threat level -> attack
// 				 * if ship.is interceptor -> find pShip with largest shield && largest threat level in range of move and attack -> move towards and attack
// 				 * if ship.is interceptor && ship.can cloak -> ship.cloak after attacking or moving
// 				 * if no pship within attack+move range of ship move towards nearest ship
// 				 * if ship.is corvette -> heal nearest ship,
// 				 * Uniques, Etc.*/
// 			switch (ship.get()->getShipType()){
// 			case ShipType::CUTTER:
// 			case ShipType::FIGHTER:
// 			case ShipType::CRUISER:
// 			case ShipType::DESTROYER:
// 			case ShipType::BATTLESHIP:
// 			{
// 
// 				break;
// 			}
// 			case ShipType::INTERCEPTOR:
// 			{
// 
// 				break;
// 			}
// 			case ShipType::BOMBER:
// 			{
// 
// 				break;
// 			}
// 			case ShipType::CORVETTE:
// 			{
// 
// 				break;
// 			}
// 			case ShipType::CARRIER:
// 			case ShipType::ASSAULT_CARRIER:
// 			{
// 				spawnCarrierChild(ship.get());
// 				/* Move and attack if there is a high enough threat level within range */
// 				/* Loop backwords because it is sorted lowest to highest threat levels */
// 				float distanceTo = 0.0f;
// 				for (int i = m_playerFleet->getFleetSize() - 1; i > 0; ++i){
// 					distanceTo = glm::length(glm::vec2(ship->getPosition() - (*m_playerFleet)[i]->getPosition()));
// 					if ((*m_playerFleet)[i]->getShipThreat() < 15 || (*m_playerFleet)[i]->getShipThreat() == 25) break;
// 					if (distanceTo < ship->getAttackRange() + ship->getSpeed()){
// 						glm::ivec2 movePos = ship->getPosition() + (glm::ivec2)glm::normalize((glm::vec2(ship->getPosition() - m_playerFleet->getShips()[i]->getPosition())) * ((distanceTo - (float)ship->getAttackRange() < 0) ? 0 : (float)ship->getSpeed()));
// 						ship->move(movePos, grid, m_playerFleet);
// 						ship->queueAttack((*m_playerFleet)[i], (ship->getShipType() == ShipType::ASSAULT_CARRIER) ? (((*m_playerFleet)[i]->getShipShield() <= 0) ? true : false) : true);
// 						break;
// 					}
// 				}
// 				break;
// 			}
// 			default:
// 				break;
// 			}
 		}
		m_hasUpdatedOnce = true;
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
#define ADD_SHIP(s_type, add_args) if(yPositions.size() > 0){\
	spawnLocation = glm::ivec2(grid->getDims().x-1, yPositions.back());\
	int s_id = m_fleet.addShip(grid, resourceManager, s_type, glm::vec2(grid->getScreenPos(spawnLocation) + (grid->getTileDims() / 2.0f)), spawnLocation, add_args, true);\
	while (s_id == -1){\
		yPositions.pop_back();\
		if(yPositions.size() <= 0) break;\
		spawnLocation = glm::ivec2(grid->getDims().x-1, yPositions.back());\
		s_id = m_fleet.addShip(grid, resourceManager, s_type, glm::vec2(grid->getScreenPos(spawnLocation) + (grid->getTileDims() / 2.0f)), spawnLocation, add_args, true);\
	}\
	if(yPositions.size() > 0) yPositions.pop_back();\
	if(m_fleet.getFleetSize() > 0) m_fleet.getShips()[s_id].get()->scaleStrength(((float)m_currentWave / (float)LEVEL6SPAWN), ((float)m_currentWave / (float)LEVEL6SPAWN) / 2.0f); }
	/***************/
	std::vector<int> yPositions;
	for (int i = 0; i < grid->getDims().y; ++i){
		yPositions.push_back(i);
	}
	std::shuffle(std::begin(yPositions), std::end(yPositions), gen);
	glm::ivec2 spawnLocation = glm::ivec2();
	int curMaxNumShips = numShipsToSpawn;
	if (currentlySpawning & LEVEL6){
		std::uniform_int_distribution<int> randL7Type(0, 1);
		while (numShipsToSpawn >= curMaxNumShips - (int)((numShipsToSpawn - LEVEL6SPAWN) / 2)){
			if (randL7Type(gen) == 0){
				//Spawn BattleShip
				ADD_SHIP(ShipType::BATTLESHIP, 0);
			}
			else {
				//Spawn Assault Carrier
				ADD_SHIP(ShipType::ASSAULT_CARRIER, 0);
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
				ADD_SHIP(ShipType::CARRIER, 0);
			}
			else {
				//Spawn Destroyer
				ADD_SHIP(ShipType::DESTROYER, 0);
			}
			--numShipsToSpawn;
		}
	}
	curMaxNumShips = numShipsToSpawn;
	if (currentlySpawning & LEVEL4){
		while (numShipsToSpawn > curMaxNumShips - ((currentlySpawning & LEVEL5SPAWN) ? 2 : 1)){
			//Spawn Corvette
			ADD_SHIP(ShipType::CORVETTE, 0);
			--numShipsToSpawn;
		}
	}
	curMaxNumShips = numShipsToSpawn;
	if (currentlySpawning & LEVEL3){
		std::uniform_int_distribution<int> randL4Type(1, 5);
		while (numShipsToSpawn > curMaxNumShips - (int)std::round((double)curMaxNumShips/ 3.0)){
			if (randL4Type(gen) == 2 || randL4Type(gen) == 4){
				//Spawn Interceptor
				ADD_SHIP(ShipType::INTERCEPTOR, 1);
			}
			else {
				//Spawn Cutter
				ADD_SHIP(ShipType::CUTTER, 0);
			}
			--numShipsToSpawn;
		}
	}
	curMaxNumShips = numShipsToSpawn;
	if (currentlySpawning & LEVEL2){
		while (numShipsToSpawn > curMaxNumShips - ((currentlySpawning & LEVEL4SPAWN) ? ((currentlySpawning & LEVEL5SPAWN) ? 3 : 2) : 1)){
			//Spawn Bomber
			ADD_SHIP(ShipType::BOMBER, 0);	
			--numShipsToSpawn;
		}
	}
	while (numShipsToSpawn && (int)yPositions.size() > 0){
		//Spawn "Fodder"
		ADD_SHIP((currentlySpawning & LEVEL4) ? ShipType::CRUISER : ShipType::FIGHTER, 0);
		--numShipsToSpawn;
	}
	yPositions.clear();
}
