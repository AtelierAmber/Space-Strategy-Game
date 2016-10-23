#pragma once

#include <vector>
#include <memory>
#include "Ship.h"

class Fleet
{
public:
	Fleet();
	~Fleet();
	
	int addShip(Grid* grid, Sakura::ResourceManager &resourceManager, std::string team, ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, float speed, int shield, int hull, int shieldDamage, int hullDamage, int damageEffectStrength, DamageEffect damageEffect = NORMAL);
	int removeShip(unsigned int shipIndex);

	/* Return true when finished with concurrent updates */
	bool update(float deltaTime, Grid* grid);

	void draw(Sakura::SpriteBatch& spriteBatch);
	void drawDebug(Sakura::DebugRenderer& debugRenderer);

	const std::vector<std::shared_ptr<Ship>>& getShips() const{ return m_ships; }
	Ship* shipAtPosition(glm::vec2 absPos);

	void setTurn(bool turn){ m_isTurn = turn; }

private:
	std::vector<std::shared_ptr<Ship>> m_ships;
	bool m_isTurn = false;
};

