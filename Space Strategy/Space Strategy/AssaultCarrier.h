#pragma once
#include "Ship.h"
class AssaultCarrier :
	public Ship
{
public:
	AssaultCarrier(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team,
		ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy);
	~AssaultCarrier();

	int createFighter(glm::ivec2 position, Grid* grid, Sakura::ResourceManager& resourceManager);

	int callUnique(void* data) override;
private:
	Grid* m_grid_Ref;
	Sakura::ResourceManager& m_resourceManager_Ref;
};

