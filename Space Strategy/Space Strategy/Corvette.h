#pragma once
#include "Ship.h"

class Corvette :
	public Ship
{
public:
	Corvette(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team,
		ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy);
	~Corvette();

	void setCurrentEffect(DamageEffect effect){ m_damageEffect = effect; }

private:
	DamageEffect m_damageEffects[3];
};

