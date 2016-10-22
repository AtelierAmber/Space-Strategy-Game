#include "Fleet.h"
#include "Grid.h"

Fleet::Fleet(){ /* Empty */}

Fleet::~Fleet(){
	m_ships.clear();
}

int Fleet::addShip(Grid* grid, Sakura::ResourceManager &resourceManager, std::string team, ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, float speed, int shield, int hull, int shieldDamage, int hullDamage, int damageEffectStrength, DamageEffect damageEffect /*= NORMAL*/){
	if (position.x > grid->getDims().x - 1 || position.y > grid->getDims().y - 1 || position.x < 0 || position.y < 0){
		return -1;
	}
	m_ships.emplace_back(new Ship(grid, this, resourceManager, team, shipType, position, enemy, speed, shield, hull, shieldDamage, hullDamage, damageEffectStrength, damageEffect));
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
		ship->draw(spriteBatch);
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
