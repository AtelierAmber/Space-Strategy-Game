#include "AssaultCarrier.h"
#include "Fleet.h"

AssaultCarrier::AssaultCarrier(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team,
	ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy){
	init(grid, fleet, resourceManager, team, shipType, position, enemy, 5, 5, 7, 5, 0, 5, 12, DamageEffect());	
}

AssaultCarrier::~AssaultCarrier(){

}

int AssaultCarrier::createFighter(glm::ivec2 position, Grid* grid, Fleet* enemyFleet, Sakura::ResourceManager& resourceManager){
	return m_fleet->addShip(grid, enemyFleet, resourceManager, ShipType::FIGHTER, position, false);
}