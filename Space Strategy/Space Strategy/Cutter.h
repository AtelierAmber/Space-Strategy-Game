#pragma once
#include "Ship.h"
class Cutter :
	public Ship
{
public:
	Cutter(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team,
		ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, bool costsCP);
	~Cutter();

	int increaseShips();
	int decreaseShips();

	void Damage(int hullDamage, int shieldDamage, DamageEffect statusEffect) override;

	void draw(Sakura::SpriteBatch& spriteBatch, bool hover) override;

private:
	int m_numShips;
};

