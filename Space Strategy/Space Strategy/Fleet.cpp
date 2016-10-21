#include "Fleet.h"

Fleet::Fleet(){ /* Empty */}

Fleet::~Fleet(){
	m_ships.clear();
}

int Fleet::addShip(Sakura::ResourceManager &resourceManager, std::string team, ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, float speed, int shield, int hull, int shieldDamage, int hullDamage, int damageEffectStrength, DamageEffect damageEffect /*= NORMAL*/){
	m_ships.emplace_back(new Ship(this, resourceManager, team, shipType, position, enemy, speed, shield, hull, shieldDamage, hullDamage, damageEffectStrength, damageEffect));
	m_ships.back()->setID(m_ships.size()-1);
}

int Fleet::removeShip(unsigned int shipIndex){
	if (shipIndex > m_ships.size()){
		m_ships.erase(m_ships.begin() + shipIndex);
	}
}
