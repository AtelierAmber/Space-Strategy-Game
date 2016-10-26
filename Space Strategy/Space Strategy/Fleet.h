#pragma once

#include <memory>
#include "Ship.h"

class MainGUI;

class Fleet
{
public:
	Fleet();
	Fleet(Fleet* enemyFleet, std::string fleetColor, MainGUI* gui, bool isEnemy) :
		m_enemyFleet(enemyFleet), m_fleetColor(fleetColor), m_gui(gui), m_isEnemy(isEnemy){}
	~Fleet();

	void init(Fleet* enemyFleet, std::string fleetColor, MainGUI* gui, bool isEnemy);
	
	int addShip(Grid* grid, Sakura::ResourceManager &resourceManager, ShipType shipType, glm::vec2 absPosition, glm::ivec2 position /* Position on GRID */, int additionalData, bool costsCP = true);
	int removeShip(unsigned int shipIndex);

	/* Return true when finished with concurrent updates */
	bool update(float deltaTime, Grid* grid);

	void draw(Sakura::SpriteBatch& spriteBatch, Grid* grid, const glm::vec2& mouseCoords);
	void drawDebug(Sakura::DebugRenderer& debugRenderer);

	std::vector<std::shared_ptr<Ship>>& getShips() { return m_ships; }
	Ship* shipAtPosition(glm::vec2 absPos);

	void setAddedShip(ShipType addedShip){ m_addedShip = addedShip; }
	const ShipType& getAddedShip() const{ return m_addedShip; }

	void setSelectedShip(Ship* ship){ 
		if (m_selectedShip){
			m_selectedShip->setSelected(false);
		}
		m_selectedShip = ship;
		if (m_selectedShip){
			m_selectedShip->setSelected(true);
		}
	}
	Ship*& getSelectedShip(){ return m_selectedShip; }

	const int getFleetSize() const{ return (int)m_ships.size(); }
	std::string getTeam(){ return m_fleetColor; }

	void sortFleet(int left, int right);
	static bool shipThreatLevel(std::shared_ptr<Ship> ship1, std::shared_ptr<Ship> ship2);

private:
	std::vector<std::shared_ptr<Ship>> m_ships;
	ShipType m_addedShip = ShipType::CUTTER;
	bool m_moving = false;
	bool m_turnFinished = false;

	MainGUI* m_gui = nullptr;
	bool m_isEnemy;
	Fleet* m_enemyFleet;
	std::string m_fleetColor = "Gray";
	Ship* m_selectedShip = nullptr;
};

