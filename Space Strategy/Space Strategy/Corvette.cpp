#include "Corvette.h"

Corvette::Corvette(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team,
	ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy){
	init(grid, fleet, resourceManager, team, shipType, position, enemy, 5, 5, 7, 5, 1, 5, 12, DamageEffect());
	m_damageEffects[0] = DamageEffect(SUPERCHARGE, 1.25f, 1.0f, 2);
	m_damageEffects[1] = DamageEffect(DAMAGEBOOST, 1.25f, 1.0f, 2);
	m_damageEffects[2] = DamageEffect(REPAIR, 2.0f, 1.0f, 2);
}

Corvette::~Corvette(){

}
