#include "AssaultCarrier.h"
#include "Fleet.h"
#include "Grid.h"

AssaultCarrier::AssaultCarrier(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team,
	ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy) : m_resourceManager_Ref(resourceManager){
	init(grid, fleet, resourceManager, team, shipType, position, enemy, 5, 5, 7, 5, 0, 5, 12, DamageEffect());
	m_grid_Ref = grid;
}

AssaultCarrier::~AssaultCarrier(){

}

int AssaultCarrier::createFighter(glm::ivec2 position, Grid* grid, Sakura::ResourceManager& resourceManager){
	return m_fleet->addShip(grid, resourceManager, ShipType::FIGHTER, grid->getScreenPos(position), position, false);
}

int AssaultCarrier::callUnique(void* data){
	glm::ivec2 position = *(static_cast<glm::ivec2*>(data));
	return createFighter(position, m_grid_Ref, m_resourceManager_Ref);
}
