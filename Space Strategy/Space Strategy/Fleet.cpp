#include "Fleet.h"
#include "Grid.h"
#include "MainGUI.h"
#include <algorithm>

/* Unique Ships */
#include "AssaultCarrier.h"
#include "Carrier.h"
#include "Corvette.h"
#include "Cutter.h"
#include "Interceptor.h"

Fleet::Fleet(){ /* Empty */}

Fleet::~Fleet(){
	m_ships.clear();
}

void Fleet::init(Fleet* enemyFleet, std::string fleetColor, MainGUI* gui, bool isEnemy){
	m_enemyFleet = enemyFleet;
	m_fleetColor = fleetColor;
	m_gui = gui;
	m_isEnemy = isEnemy;
}

int Fleet::addShip(Grid* grid, Sakura::ResourceManager &resourceManager, ShipType shipType, glm::vec2 absPosition, glm::ivec2 position /* Position on GRID */, int additionalData, bool costsCP){
	if (position.x > grid->getDims().x - 1 || position.y > grid->getDims().y - 1 || position.x < 0 || position.y < 0){
		return -1;
	}
	if (shipAtPosition(absPosition) || m_enemyFleet->shipAtPosition(absPosition)){
		return -1;
	}
	if (((Ship::getTypeCost(shipType) * (int)costsCP) > (m_gui->getMaxCP() - m_gui->getUsedCP())) && !m_isEnemy){
		return -2;
	}

	int placementDir = (m_enemyFleet) ? -1 : 1;

	/* Ship Base Stats (speed, shield, hull, shield damage, hull damage, range, damage effect(strength, chance), cost)
	 * Cutter: low shield, low health, very fast, travels in groups, attacks on have a 1/groupSize % chance to hit
	 * -> 10, 2, 2, 1, 1, 1, Normal, 1CP
	 * Fighter: low shield, average health, very fast
	 * -> 8, 3, 5, 1, 1, 2, Normal, 2CP
	 * Interceptor: average shield, low health, very fast, can cloak, can shut down enemy shields
	 * -> 10, 5, 2, 1, 0, 3, PowerShortage(2, 5%), 5CP
	 * Bomber: average shield, high health, slow, long range high damage attacks, can set fire to ships
	 * -> 2, 5, 2, 1, 1, 10, Fire(5, 15%), 10CP
	 * Corvette: average shield, average health, can boost other ships, damage enemy shields
	 * -> 5, 5, 7, 5, 0, 5, SuperCharge(125%, 100%) | DamageBoost(125%, 100%) | Repair(2, 100%), 12CP
	 * Cruiser: average shield, average health, average ship
	 * -> 5, 7, 7, 3, 3, 3, Normal, 12CP
	 * Carrier: average shield, average health, can deploy cutter units with no cost to CP, and disable enemy systems
	 * -> 5, 7, 7, 0, 0, 2, Emp(2, 25%), 15CP
	 * Destroyer: high shield, high health, high hull damage, can set fire to ships, slow
	 * -> 3, 10, 10, 2, 7, 3, Fire(5, 15%), 20CP
	 * Assault Carrier: high shield, average health, slow, can deploy fighters with no cost to CP
	 * -> 3, 15, 7, 3, 3, 3, Normal, 30CP
	 * Battleship: Mother of all ships, high shield, high health, slow, high damage, can set fire to ships
	 * -> 3, 15, 15, 7, 7, 3, Fire(5, 25%), 40CP 
	 */
	switch (shipType)
	{
	case ShipType::CUTTER:
		//! TODO IMPLEMENT SEPERATE SHIP CLASSES
		m_ships.emplace_back(new Cutter(grid, this, resourceManager, m_fleetColor, shipType, position, m_isEnemy, costsCP));
		break;
	case ShipType::FIGHTER:
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_isEnemy, 8, 3, 5, 1, 1, 2, 2 * (int)costsCP, DamageEffect()));
		break;
	case ShipType::INTERCEPTOR:
		m_ships.emplace_back(new Interceptor(grid, this, resourceManager, m_fleetColor, shipType, position, m_isEnemy, additionalData != 0));
		break;
	case ShipType::BOMBER:
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_isEnemy, 2, 5, 2, 1, 1, 10, 10 * (int)costsCP, DamageEffect(FIRE, 1.0f, 0.15f, 5)));
		break;
	case ShipType::CORVETTE:
		m_ships.emplace_back(new Corvette(grid, this, resourceManager, m_fleetColor, shipType, position, m_isEnemy));
		break;
	case ShipType::CRUISER:
		if (shipAtPosition(glm::vec2(absPosition.x + (placementDir * grid->getTileDims().x), absPosition.y)) || m_enemyFleet->shipAtPosition(glm::vec2(absPosition.x + (placementDir * grid->getTileDims().x), absPosition.y))){
			return -1;
		}
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_isEnemy, 5, 7, 7, 3, 3, 3, 12 * (int)costsCP, DamageEffect()));
		break;
	case ShipType::CARRIER:
		if (shipAtPosition(glm::vec2(absPosition.x + (placementDir * grid->getTileDims().x), absPosition.y)) || m_enemyFleet->shipAtPosition(glm::vec2(absPosition.x + (placementDir * grid->getTileDims().x), absPosition.y))){
			return -1;
		}
		m_ships.emplace_back(new Carrier(grid, this, resourceManager, m_fleetColor, shipType, position, m_isEnemy));
		break;
	case ShipType::DESTROYER:
		if (shipAtPosition(glm::vec2(absPosition.x + (placementDir * grid->getTileDims().x), absPosition.y)) || m_enemyFleet->shipAtPosition(glm::vec2(absPosition.x + (placementDir * grid->getTileDims().x), absPosition.y))){
			return -1;
		}
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_isEnemy, 3, 10, 10, 2, 7, 3, 20 * (int)costsCP, DamageEffect(FIRE, 1.0f, 0.15f, 5)));
		break;
	case ShipType::ASSAULT_CARRIER:
		if (shipAtPosition(glm::vec2(absPosition.x + (placementDir * grid->getTileDims().x), absPosition.y)) || m_enemyFleet->shipAtPosition(glm::vec2(absPosition.x + (placementDir * grid->getTileDims().x), absPosition.y))){
			return -1;
		}
		m_ships.emplace_back(new AssaultCarrier(grid, this, resourceManager, m_fleetColor, shipType, position, m_isEnemy));
		break;
	case ShipType::BATTLESHIP:
		if (shipAtPosition(glm::vec2(absPosition.x + (placementDir * grid->getTileDims().x), absPosition.y)) || m_enemyFleet->shipAtPosition(glm::vec2(absPosition.x + (placementDir * grid->getTileDims().x), absPosition.y))){
			return -1;
		}
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_isEnemy, 3, 15, 15, 7, 7, 3, 40 * (int)costsCP, DamageEffect(FIRE, 1.0f, 0.25f, 5)));
		break;
	case ShipType::COMMANDSHIP:
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_isEnemy, 0, 25, 25, 0, 0, 0, 0, DamageEffect()));
		break;
	default:
		return -3;
		break;
	}
	m_ships.back()->setID(m_ships.size()-1);
	if (!m_isEnemy) m_gui->addUsedCP(Ship::getTypeCost(shipType) * (int)costsCP);
	sortFleet();
	return 0;
}

int Fleet::removeShip(unsigned int shipIndex){
	if (shipIndex < m_ships.size()){
		if (m_selectedShip && m_selectedShip->getID() == shipIndex){
			m_selectedShip = nullptr;
		}
		if (m_isEnemy) { 
			m_gui->addScore(5 * m_ships[shipIndex]->getCost());
		}
		else{
			m_gui->addScore(-5 * m_ships[shipIndex]->getCost());
			m_gui->addUsedCP(-1 * m_ships[shipIndex]->getCost());
		}
		m_ships[shipIndex] = m_ships.back();
		m_ships[shipIndex]->setID(shipIndex);
		m_ships.pop_back();
		sortFleet();
		return 0;
	}
	return -1;
}

bool Fleet::update(float deltaTime, Grid* grid){
	for (std::size_t i = 0; i < m_ships.size(); ++i){
		if (m_ships[i]->update(deltaTime, grid)){
			--i;
		}
	}
	m_moving = false;
	for (auto& ship : m_ships){
		if (!ship->isMoveFinished()){
			m_moving |= !ship->updateMove(deltaTime, grid);
		}
	}
	if (!m_moving && !m_turnFinished){
		for (auto& ship : m_ships){
			ship->updateAttack();
		}
		m_turnFinished = true;
	}
	if (m_turnFinished){
		m_turnFinished = false;
		for (auto& ship : m_ships){
			ship->update(deltaTime, grid);
			ship->endTurn();
		}
		return true;
	}
	return false;
}

void Fleet::draw(Sakura::SpriteBatch& spriteBatch, Grid* grid, const glm::vec2& mouseCoords){
	for (auto& ship : m_ships){
		ship->draw(spriteBatch, grid, ship->collidesPoint(mouseCoords));
	}
}

void Fleet::drawDebug(Sakura::DebugRenderer& debugRenderer){
	for (auto& ship : m_ships){
		ship->drawDebug(debugRenderer);
	}
}

Ship* Fleet::shipAtPosition(glm::vec2 absPos){
	for (std::size_t i = 0; i < m_ships.size(); ++i){
		if (m_ships[i]->collidesPoint(absPos)){
			return m_ships[i].get();
		}
	}
	return nullptr;
}

void Fleet::sortFleet(){
	std::sort(m_ships.begin(), m_ships.end(), shipThreatLevel);
}

bool Fleet::shipThreatLevel(std::shared_ptr<Ship> ship1, std::shared_ptr<Ship> ship2){
	return (ship1.get()->getShipThreat() > ship2.get()->getShipThreat());
}

