#include "Fleet.h"
#include "Grid.h"

Fleet::Fleet(){ /* Empty */}

Fleet::~Fleet(){
	m_ships.clear();
}

void Fleet::init(bool isEnemy, std::string fleetColor){
	m_enemyFleet = isEnemy;
	m_fleetColor = fleetColor;
}

int Fleet::addShip(Grid* grid, Fleet* enemyFleet, Sakura::ResourceManager &resourceManager, ShipType shipType, glm::ivec2 position /* Position on GRID */){
	if (position.x > grid->getDims().x - 1 || position.y > grid->getDims().y - 1 || position.x < 0 || position.y < 0){
		return -1;
	}
	if (shipAtPosition(grid->getScreenPos(position)) || enemyFleet->shipAtPosition(grid->getScreenPos(position))){
		return -1;
	}
	/* Ship Base Stats (speed, shield, hull, shield damage, hull damage, range, damage effect(strength, chance), cost)
	 * Cutter: low shield, low health, very fast, travels in groups, can evade attacks with a 1/groupSize % chance
	 * -> 10, 2, 2, 1, 1, 1, Normal, 1CP
	 * Fighter: low shield, average health, very fast
	 * -> 8, 3, 5, 1, 1, 2, Normal, 2CP
	 * Interceptor: average shield, low health, very fast, can cloak, can shut down enemy shields
	 * -> 10, 5, 2, 1, 0, 3, PowerShortage(2, 5%), 5CP
	 * Bomber: average shield, high health, slow, long range high damage attacks, can set fire to ships
	 * -> 2, 5, 2, 1, 1, 10, Fire(5, 15%), 10CP
	 * Corvette: average shield, average health, can boost other ships, damage enemy shields
	 * -> 5, 5, 7, 5, null, 5, SuperCharge(125%, 100%) | DamageBoost(125%, 100%) | Repair(2, 100%), 12CP
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
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_enemyFleet, 10.0f, 2, 2, 1, 1, 1, 1, 0, 0.0f, NORMAL));
		break;
	case ShipType::FIGHTER:
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_enemyFleet, 8, 3, 5, 1, 1, 2, 2, 0, 0.0f, NORMAL));
		break;
	case ShipType::INTERCEPTOR:
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_enemyFleet, 10, 5, 2, 1, 0, 3, 5, 2, 0.05f, POWERSHORTAGE));
		break;
	case ShipType::BOMBER:
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_enemyFleet, 2, 5, 2, 1, 1, 10, 10, 5, 0.15f, FIRE));
		break;
	case ShipType::CORVETTE:
		//! TODO IMPLEMENT SEPERATE SHIP CLASSES
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_enemyFleet, 5, 5, 7, 5, 0, 5, 12, 0, 0.0f, NORMAL));
		break;
	case ShipType::CRUISER:
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_enemyFleet, 5, 7, 7, 3, 3, 3, 12, 0, 0.0f, NORMAL));
		break;
	case ShipType::CARRIER:
		//! TODO IMPLEMENT SEPERATE SHIP CLASSES
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_enemyFleet, 5, 7, 7, 0, 0, 2, 15, 2, 0.25f, EMP));
		break;
	case ShipType::DESTROYER:
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_enemyFleet, 3, 10, 10, 2, 7, 3, 20, 5, 0.15f, FIRE));
		break;
	case ShipType::ASSAULT_CARRIER:
		//! TODO IMPLEMENT SEPERATE SHIP CLASSES
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_enemyFleet, 3, 15, 7, 3, 3, 3, 20, 0, 0.0f, NORMAL));
		break;
	case ShipType::BATTLESHIP:
		m_ships.emplace_back(new Ship(grid, this, resourceManager, m_fleetColor, shipType, position, m_enemyFleet, 3, 15, 15, 7, 7, 3, 5, 0.25f, FIRE));
		break;
	default:
		return -2;
		break;
	}
	m_ships.back()->setID(m_ships.size()-1);
	return 0;
}

int Fleet::removeShip(unsigned int shipIndex){
	if (shipIndex > m_ships.size()){
		m_ships.erase(m_ships.begin() + shipIndex);
		return 0;
	}
	return -1;
}

bool Fleet::update(float deltaTime, Grid* grid){
	for (auto& ship : m_ships){
		ship->update(deltaTime, m_isTurn, grid);
	}
	return true;
}

void Fleet::draw(Sakura::SpriteBatch& spriteBatch){
	for (auto& ship : m_ships){
		ship->draw(spriteBatch, true);
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
