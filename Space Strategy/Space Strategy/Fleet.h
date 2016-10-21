#pragma once

#include <vector>
#include <memory>
#include "Ship.h"

class Fleet
{
public:
	Fleet();
	~Fleet();
	
	int addShip(Sakura::ResourceManager &resourceManager, std::string team, ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, float speed, int shield, int hull, int shieldDamage, int hullDamage, int damageEffectStrength, DamageEffect damageEffect = NORMAL);
	int removeShip(unsigned int shipIndex);

	const std::vector<std::shared_ptr<Ship>>& getShips(){ return m_ships; }

private:
	std::vector<std::shared_ptr<Ship>> m_ships;
};

