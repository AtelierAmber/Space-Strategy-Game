#include "Ship.h"
#include "Grid.h"
#include "Fleet.h"
#include <algorithm>

#define PI 3.141592

Ship::Ship(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team, 
	ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, int speed, 
	int shield, int hull, int shieldDamage, int hullDamage, int range, CP CPcost, DamageEffect damageEffect){
	init(grid, fleet, resourceManager, team, shipType, position, enemy, speed, shield, hull, shieldDamage, hullDamage, range, CPcost, damageEffect);
}

Ship::~Ship(){

}

void Ship::init(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team, 
	ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, int speed, int shield, 
	int hull, int shieldDamage, int hullDamage, int range, CP CPcost, DamageEffect damageEffect){

	std::string texturePath = "Assets/Sprites/Ships/" + team + "/" + getShipName(shipType) + ".png";
	glm::ivec2 tileDims = (shipType == ShipType::CUTTER) ? glm::ivec2(3, 1) : glm::ivec2(1, 1);
	m_texture = resourceManager.getTileSheet(texturePath.c_str(), tileDims, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_shieldTex = resourceManager.getTexture("Assets/Sprites/Ships/shield.png", MIPMAP | PIXELATED | TRANS_BORDER);
	m_hearts = resourceManager.getTileSheet("Assets/Sprites/UI/ship_health.png", glm::ivec2(3,1), MIPMAP | PIXELATED | EDGE_CLAMP);
	//HACK Temporary heart display. Display hearts in fleet informational window
	m_trailMarkers = resourceManager.getTileSheet("Assets/Sprites/UI/ship_trail.png", glm::ivec2(2, 1), MIPMAP | PIXELATED | EDGE_CLAMP);
	m_attackMarkers = resourceManager.getTileSheet("Assets/Sprites/UI/attack_marker.png", glm::ivec2(2, 1), MIPMAP | PIXELATED | EDGE_CLAMP);
	m_fleet = fleet;
	m_team = team;
	m_speed = speed;
	m_shipType = shipType;
	m_position = position;
	if (enemy){
		--m_position.x;
	}
	m_newPosition = m_position;
	m_enemy = enemy;
	m_shieldMax= shield;
	m_shield = shield;
	m_hullMax= hull;
	m_hull = hull;
	m_shieldDamage = shieldDamage;
	m_hullDamage = hullDamage;
	m_range = range;
	m_CPcost = CPcost;
	m_damageEffect = damageEffect;
	if (shipType == ShipType::DESTROYER || shipType == ShipType::ASSAULT_CARRIER || shipType == ShipType::CARRIER || shipType == ShipType::BATTLESHIP || shipType == ShipType::CRUISER){
		m_tileSpan = glm::vec2(2, 1);
	}
	else if (shipType == ShipType::COMMANDSHIP){
		m_tileSpan = glm::vec2(2, 3);
	}
	else m_tileSpan = glm::vec2(1, 1);
	m_threatLevel = (float)m_CPcost;
	if (shipType == ShipType::COMMANDSHIP){
		m_threatLevel = 25;
	}
	m_bounds.initialize(grid->getScreenPos(m_position).x, grid->getScreenPos(m_position).y, m_tileSpan.x * grid->getTileDims().x, m_tileSpan.y * grid->getTileDims().y, true);
}

int Ship::destroy(){
	return m_fleet->removeShip(this->m_id);
}

int Ship::update(float deltaTime, Grid* grid){
	if (!m_hasUpdatedOnce){
		calculateFriendlyEffects();
		if (calculateBadEffects()){
			return -1;
		}
		m_hasUpdatedOnce = true;
	}
	m_position = grid->getGridPosClamped(glm::vec2(m_bounds.x1, m_bounds.y2));
	return 0;
}

int signOf(int f){
	return (f >= 0) ? 1 : -1;
}

/* returns true if move is finished, false if the move needs to be updated still */
bool Ship::updateMove(float deltaTime, Grid* grid){
	if (m_position != m_newPosition){
		if (m_moveCounter == 0){
			m_moveCounter = (int)m_speed;
			if (m_moveCounter == 0){
				return true;
			}
			m_moveFinished = false;
		}
		glm::ivec2 distToNew = m_newPosition - m_position;
		glm::vec2 distToSpeed = glm::normalize((glm::vec2)distToNew) * (float)m_speed;
		if (abs(distToSpeed.x) > abs(distToSpeed.y)){
			m_bounds.move(grid->getTileDims().x * signOf(distToNew.x), 0.0f);
		}
		else if (abs(distToSpeed.y) > abs(distToSpeed.x)){
			m_bounds.move(0.0f, grid->getTileDims().y * signOf(distToNew.y));
		}
		else {
			m_bounds.move(grid->getTileDims().x * signOf(distToNew.x), grid->getTileDims().y * signOf(distToNew.y));
		}
		--m_moveCounter;
	}
	else {
		m_moveCounter = 0;
		m_moveFinished = true;
		return true;
	}
	if (m_moveCounter == 0){
		m_moveFinished = true;
		return true;
	}
	return false;
}

void Ship::updateAttack(){
	if (m_queuedAttack){
		damageOther(m_queuedAttack);
	}
}

void Ship::endTurn(){
	m_hasUpdatedOnce = false;
	m_moveFinished = false;
}

void Ship::calculateFriendlyEffects(){
	for (std::size_t i = 0; i < m_appliedEffects.size(); ++i){
		switch (m_appliedEffects[i].effect){
		case SUPERCHARGE:
			if (m_appliedEffects[i].duration > 0){
				m_shield = (int)std::round((float)m_shield * m_appliedEffects[i].strength);
				--m_appliedEffects[i].duration;
			}
			else {
				m_shield = (int)std::round((float)m_shield / m_appliedEffects[i].strength);
				m_appliedEffects.erase(m_appliedEffects.begin() + i);
				--i;
			}
			break;
		case DAMAGEBOOST:
			if(m_appliedEffects[i].duration > 0){
				m_shieldDamage = (int)std::round((float)m_shieldDamage * m_appliedEffects[i].strength);
				m_hullDamage = (int)std::round((float)m_hullDamage * m_appliedEffects[i].strength);
				--m_appliedEffects[i].duration;
			}
			else {
				m_shieldDamage = (int)std::round((float)m_shieldDamage / m_appliedEffects[i].strength);
				m_hullDamage = (int)std::round((float)m_hullDamage / m_appliedEffects[i].strength);
				m_appliedEffects.erase(m_appliedEffects.begin() + i);
				--i;
			}
			break;
		case REPAIR:
			if (m_appliedEffects[i].duration > 0){
				m_hull += (int)m_appliedEffects[i].strength;
				if (m_hull > m_hullMax){
					m_hull = m_hullMax;
				}
				--m_appliedEffects[i].duration;
			}
			else {
				m_appliedEffects.erase(m_appliedEffects.begin() + i);
				--i;
			}
			break;
		default:
			break;
		}
	}
}

int Ship::calculateBadEffects(){
	for (std::size_t i = 0; i < m_appliedEffects.size(); ++i){
		switch (m_appliedEffects[i].effect){
		case FIRE:
			if (m_appliedEffects[i].duration > 0){
				m_hull -= (int)m_appliedEffects[i].strength;
				--m_appliedEffects[i].duration;
				if (m_hull <= 0){
					return !destroy();
				}
			}
			else {
				m_appliedEffects.erase(m_appliedEffects.begin() + i);
				--i;
			}
			break;
		case EMP:
			if (m_appliedEffects[i].duration > 0){
				m_canAttack = false;
				--m_appliedEffects[i].duration;
			}
			else {
				m_canAttack = true;
				m_appliedEffects.erase(m_appliedEffects.begin() + i);
				--i;
			}
			break;
		case POWERSHORTAGE:
			if (m_appliedEffects[i].duration > 0){
				m_shield = 0;
				--m_appliedEffects[i].duration;
			}
			else {
				m_shield = m_shieldMax;
				m_appliedEffects.erase(m_appliedEffects.begin() + i);
				--i;
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

void Ship::move(const glm::ivec2& newPosition, Grid* grid, Fleet* enemyFleet){
	if (!m_fleet->shipAtPosition(glm::vec2(grid->getScreenPos(newPosition) + (grid->getTileDims() / 2.0f))) && !enemyFleet->shipAtPosition(glm::vec2(grid->getScreenPos(newPosition) + (grid->getTileDims() / 2.0f)))){
		
		m_newPosition = newPosition; 
		if (m_newPosition.x > grid->getDims().x-1){
			m_newPosition.x = grid->getDims().x-1;
		}
		if (m_newPosition.y > grid->getDims().x-1){
			m_newPosition.y = grid->getDims().x-1;
		}
		if (m_newPosition.x < 0){
			m_newPosition.x = 0;
		}
		if (m_newPosition.y < 0){
			m_newPosition.y = 0;
		}
		m_moveFinished = false;
	}
}

const std::string Ship::getShipName(ShipType shipType){
	switch (shipType)
	{
	case ShipType::CUTTER:
		return "cutter";
		break;
	case ShipType::FIGHTER:
		return "fighter";
		break;
	case ShipType::INTERCEPTOR:
		return "interceptor";
		break;
	case ShipType::BOMBER:
		return "bomber";
		break;
	case ShipType::CORVETTE:
		return "corvette";
		break;
	case ShipType::CRUISER:
		return "cruiser";
		break;
	case ShipType::CARRIER:
		return "carrier";
		break;
	case ShipType::DESTROYER:
		return "destroyer";
		break;
	case ShipType::ASSAULT_CARRIER:
		return "assault_carrier";
		break;
	case ShipType::BATTLESHIP:
		return "battleship";
		break;
	case ShipType::COMMANDSHIP:
		return "commandship";
		break;
	default:
		return "null_ship";
		break;
	}
}

const CP Ship::getTypeCost(ShipType shipType){
	switch (shipType)
	{
	case ShipType::CUTTER:
		return 1;
		break;
	case ShipType::FIGHTER:
		return 2;
		break;
	case ShipType::INTERCEPTOR:
		return 5;
		break;
	case ShipType::BOMBER:
		return 10;
		break;
	case ShipType::CORVETTE:
		return 12;
		break;
	case ShipType::CRUISER:
		return 12;
		break;
	case ShipType::CARRIER:
		return 15;
		break;
	case ShipType::DESTROYER:
		return 20;
		break;
	case ShipType::ASSAULT_CARRIER:
		return 30;
		break;
	case ShipType::BATTLESHIP:
		return 50;
		break;
	default:
		return 0;
		break;
	}
}

void Ship::scaleStrength(float healthScaler, float damageScaler){
	if (healthScaler > 1.0f){
		m_shield = (int)((float)m_shield * healthScaler);
		m_shieldMax = (int)((float)m_shieldMax * healthScaler);
		m_hull = (int)((float)m_hull * healthScaler);
		m_hullMax = (int)((float)m_hullMax * healthScaler);
	}
	if (damageScaler > 1.0f){
		m_shieldDamage = (int)((float)m_shieldDamage * damageScaler);
		m_hullDamage = (int)((float)m_hullDamage * damageScaler);
	}
}

#define heart_spacing 2
void Ship::draw(Sakura::SpriteBatch& spriteBatch, Grid* grid, bool hover){
	glm::vec4 uvRect = m_texture.getUVs(0);
	if (m_enemy){
		uvRect = m_texture.getUVs(0);
		uvRect.x += 1.0f / m_texture.dims.x;
		uvRect.z *= -1;
	}
	float shipScale = std::min(m_bounds.width / m_tileSpan.x / (m_texture.texture.width / m_texture.dims.x), m_bounds.height / m_tileSpan.x / (m_texture.texture.height / m_texture.dims.y));
	glm::vec2 shipSize = glm::vec2((m_texture.texture.width / m_texture.dims.x) * shipScale * m_tileSpan.x, (m_texture.texture.height / m_texture.dims.y) * shipScale * m_tileSpan.y + 8.0f);
	glm::vec4 destRect = glm::vec4(m_bounds.x1 + ((m_bounds.width / 2.0f) - (shipSize.x / 2.0f)), m_bounds.y2 + ((m_bounds.height / 2.0f) - (shipSize.y / 2.0f)), shipSize.x, shipSize.y);
	if (m_isSelected){
		float scaler = 4 * std::abs(std::sin(m_selectedSin));
		destRect.x -= scaler/2;
		destRect.y -= scaler/2;
		destRect.z += scaler;
		destRect.w += scaler;
		m_selectedSin += 0.05f;
	}
	spriteBatch.draw(destRect, uvRect, m_texture.texture.id, 0.0f, Sakura::ColorRGBA8(255,255,255,255));
	if (m_shield){
		spriteBatch.draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_shieldTex.id, 0.1f, Sakura::ColorRGBA8(255, 255, 255, 255));
	}
	if (hover && m_shipType != ShipType::COMMANDSHIP){
#define health_scale 2.0f
		destRect = glm::vec4(m_bounds.x1 + ((m_tileSpan.x-1) * (m_bounds.width / m_tileSpan.x)) - ((std::min(std::max(m_hullMax, m_shieldMax), 16) * (m_hearts.texture.width / 3.0f) * health_scale) / 2.0f), m_bounds.y1 + 5.0f, m_hearts.texture.width / 3.0f * health_scale, m_hearts.texture.height * health_scale);
		for (int i = 0; i < m_hullMax; ++i){
			int uv = (i < m_hull) ? 1 : 2;
			if (!(i % 15) && i != 0){
				destRect.y += (destRect.w + heart_spacing);
				destRect.x -= 14 * (destRect.z + heart_spacing);
			}else destRect.x += (destRect.z + heart_spacing);
			spriteBatch.draw(destRect, m_hearts.getUVs(uv), m_hearts.texture.id, 10.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		}
		destRect.x = m_bounds.x1 + ((m_tileSpan.x - 1) * (m_bounds.width / m_tileSpan.x)) - ((std::min(std::max(m_hullMax, m_shieldMax), 16) * (m_hearts.texture.width / 3.0f) * health_scale) / 2.0f);
		destRect.y = m_bounds.y1 + 5.0f;
		for (int i = 0; i < m_shield; ++i){
			if (!(i % 15) && i != 0){
				destRect.y += (destRect.w + heart_spacing);
				destRect.x -= 14 * (destRect.z + heart_spacing);
			}
			else destRect.x += (destRect.z + heart_spacing);
			spriteBatch.draw(destRect, m_hearts.getUVs(0), m_hearts.texture.id, 10.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		}
	}
	if (m_position != m_newPosition && !m_enemy){
		drawTravelTrail(spriteBatch, grid);
	}
	if (m_queuedAttack && !m_enemy){
		drawAttackTrail(spriteBatch, grid);
	}
}

void Ship::drawTravelTrail(Sakura::SpriteBatch& spriteBatch, Grid* grid){
	glm::vec2 travelDist = (grid->getScreenPos(m_newPosition) + (grid->getTileDims() / 2.0f)) - (grid->getScreenPos(m_position) + (grid->getTileDims() / 2.0f * (glm::vec2)m_tileSpan));
	glm::vec2 travelDir = glm::normalize(travelDist);
	glm::vec2 additionalDist;
	if (m_newPosition.x < m_position.x){
		additionalDist.x = 1;
	}
	if (m_newPosition.y < m_position.y){
		additionalDist.y = 1;
	}
	float travelLength = glm::length((grid->getScreenPos(m_newPosition) - ((grid->getTileDims() / 1.5f) * additionalDist)) - grid->getScreenPos(m_position));
	int numMarkers = (int)travelLength / (m_trailMarkers.texture.width + 5);
	glm::vec4 destRect = glm::vec4(m_bounds.x1 + (m_bounds.width / 2), m_bounds.y2 + (m_bounds.height / 2), m_trailMarkers.texture.width, m_trailMarkers.texture.height*2);
	int uv = 0;
	for (int i = 0; i < numMarkers; ++i){
		destRect.x += (m_trailMarkers.texture.width + 5.0f) * travelDir.x;
		destRect.y += (m_trailMarkers.texture.height * 2 + 5.0f) * travelDir.y;
		uv = (i < m_speed * 2.5 - 2) ? 0 : 1;
		spriteBatch.draw(destRect, m_trailMarkers.getUVs(uv), m_trailMarkers.texture.id, -0.5f, Sakura::ColorRGBA8(255, 255, 255, 255), travelDir);
	}
}

void Ship::drawAttackTrail(Sakura::SpriteBatch& spriteBatch, Grid* grid){
	glm::vec2 travelDist = (grid->getScreenPos(m_queuedAttack->getPosition()) + (grid->getTileDims() / 2.0f * (glm::vec2)m_queuedAttack->getTileSpan())) - (grid->getScreenPos(m_position) + (grid->getTileDims() / 2.0f * (glm::vec2)m_tileSpan));
	glm::vec2 travelDir = glm::normalize(travelDist);
	glm::vec2 additionalDist;
	if (m_queuedAttack->getPosition().x < m_position.x){
		additionalDist.x = 1;
	}
	if (m_queuedAttack->getPosition().y < m_position.y){
		additionalDist.y = 1;
	}
	float travelLength = glm::length((grid->getScreenPos(m_queuedAttack->getPosition()) - ((grid->getTileDims() / 2.0f) * additionalDist)) - grid->getScreenPos(m_position));
	int numMarkers = (int)travelLength / (m_attackMarkers.texture.width + 5);
	glm::vec4 destRect = glm::vec4(m_bounds.x1 + (m_bounds.width / 2), m_bounds.y2 + (m_bounds.height / 2), m_attackMarkers.texture.width, m_attackMarkers.texture.height * 2);
	int uv = 0;
	for (int i = 0; i < numMarkers - 1; ++i){
		destRect.x += (m_attackMarkers.texture.width + 5.0f) * travelDir.x;
		destRect.y += (m_attackMarkers.texture.height * 2 + 5.0f) * travelDir.y;
		uv = (i < m_range * 3 - 2) ? 0 : 1;
		spriteBatch.draw(destRect, m_attackMarkers.getUVs(uv), m_attackMarkers.texture.id, -0.5f, Sakura::ColorRGBA8(255, 255, 255, 255), travelDir);
	}
}

void Ship::drawDebug(Sakura::DebugRenderer& debugRenderer){
	debugRenderer.drawBox(glm::vec4(m_bounds.x1, m_bounds.y2, m_bounds.width, m_bounds.height), Sakura::ColorRGBA8(255, 0, 0, 255), 0);
}

int Ship::Damage(int hullDamage, int shieldDamage, DamageEffect statusEffect /*= NORMAL*/){
	if (statusEffect.effect != FIRE){
		ApplyEffect(statusEffect);
	}
	if (m_shield <= 0){
		m_shield = 0;
		m_hull -= hullDamage;
		if (m_hull <= 0){
			return destroy();
		}
		if (statusEffect.effect == FIRE){
			ApplyEffect(statusEffect);
		}
	}
	m_shield -= shieldDamage;
	return -1;
}

void Ship::ApplyEffect(DamageEffect statusEffect){
	if (statusEffect.effect != NORMAL){
		if (statusEffect.applyEffect()){
			m_appliedEffects.push_back(statusEffect);
		}
	}
}

void Ship::damageOther(Ship* otherShip){
	int distanceTo = (int)glm::length(glm::vec2(m_position - otherShip->getPosition()));
	if (distanceTo < m_range){
		if (otherShip){
			otherShip->Damage(m_hullDamage, m_shieldDamage, m_damageEffect);
		}
	}
	m_queuedAttack = nullptr;
}