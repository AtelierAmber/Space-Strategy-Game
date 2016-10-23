#pragma once

#include <vector>
#include <memory>
#include "Ship.h"

class Fleet
{
public:
	Fleet();
	Fleet(bool isEnemy, std::string fleetColor) : m_enemyFleet(isEnemy), m_fleetColor(fleetColor){}
	~Fleet();

	void init(bool isEnemy, std::string fleetColor);
	
	int addShip(Grid* grid, Fleet* enemyFleet, Sakura::ResourceManager &resourceManager, ShipType shipType, glm::ivec2 position /* Position on GRID */, int additionalData, bool costsCP = true);
	int removeShip(unsigned int shipIndex);

	/* Return true when finished with concurrent updates */
	bool update(float deltaTime, Grid* grid);

	void draw(Sakura::SpriteBatch& spriteBatch);
	void drawDebug(Sakura::DebugRenderer& debugRenderer);

	const std::vector<std::shared_ptr<Ship>>& getShips() const{ return m_ships; }
	Ship* shipAtPosition(glm::vec2 absPos);

	void setTurn(bool turn){ m_isTurn = turn; }
	bool getTurn(){ return m_isTurn; }
	void setAddedShip(ShipType addedShip){ m_addedShip = addedShip; }
	const ShipType& getAddedShip() const{ return m_addedShip; }

	void setSelectedShip(Ship* ship){ m_selectedShip = ship; }
	Ship*& getSelectedShip(){ return m_selectedShip; }

private:
	std::vector<std::shared_ptr<Ship>> m_ships;
	ShipType m_addedShip = ShipType::NOSHIP;
	bool m_isTurn = false;
	bool m_enemyFleet = false;
	std::string m_fleetColor = "Gray";
	Ship* m_selectedShip = nullptr;
};

