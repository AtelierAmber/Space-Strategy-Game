#pragma once
#include "Ship.h"
class Carrier :
	public Ship
{
public:
	Carrier(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team,
		ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy);
	~Carrier();

	int createCutter(glm::ivec2 position, Grid* grid, Sakura::ResourceManager& resourceManager);
};

