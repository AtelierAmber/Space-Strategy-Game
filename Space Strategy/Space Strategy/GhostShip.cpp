#include "GhostShip.h"
#include "Grid.h"
#include "Fleet.h"
#include <algorithm>

GhostShip::~GhostShip(){

}

void GhostShip::update(Grid* grid, glm::vec2 mouseCoords){
	m_position = grid->getGridPos(mouseCoords);
	m_bounds.x1 = grid->getScreenPos(m_position).x;
	m_bounds.y2 = grid->getScreenPos(m_position).y;
}

void GhostShip::draw(Sakura::SpriteBatch& spriteBatch, Fleet* playerFleet, Fleet* enemyFleet){
	glm::vec4 uvRect = m_texture.getUVs(0);
	float shipScale = std::min(m_bounds.width / m_tileSpan.x / (m_texture.texture.width / m_texture.dims.x), m_bounds.height / m_tileSpan.x / (m_texture.texture.height / m_texture.dims.y));
	glm::vec2 shipSize = glm::vec2((m_texture.texture.width / m_texture.dims.x) * shipScale * m_tileSpan.x, (m_texture.texture.height / m_texture.dims.y) * shipScale * m_tileSpan.y);
	glm::vec4 destRect = glm::vec4(m_bounds.x1 + ((m_bounds.width / 2.0f) - (shipSize.x / 2.0f)), m_bounds.y2 + ((m_bounds.height / 2.0f) - (shipSize.y / 2.0f)), shipSize.x, shipSize.y);
	int greenVal = (checkCollision(playerFleet) || checkCollision(enemyFleet)) ? 100 : 255;
	int blueVal = greenVal;
	spriteBatch.draw(destRect, uvRect, m_texture.texture.id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, 150));
}

bool GhostShip::checkCollision(Fleet* fleet){
	for (auto& ship : fleet->getShips()){
		if (ship->collidesRect(m_bounds)){
			return true;
		}
	}
	return false;
}

void GhostShip::setShipType(ShipType shipType, Sakura::ResourceManager &resourceManager, const std::string& team, Grid* grid){
	m_shipType = shipType;
	m_team = team;
	std::string texturePath = "Assets/Sprites/Ships/" + team + "/" + Ship::getShipName(shipType) + ".png";
	glm::ivec2 tileDims = (shipType == ShipType::CUTTER) ? glm::ivec2(3, 1) : glm::ivec2(1, 1);
	m_texture = resourceManager.getTileSheet(texturePath.c_str(), tileDims, MIPMAP | PIXELATED | EDGE_CLAMP);
	if (shipType == ShipType::DESTROYER || shipType == ShipType::ASSAULT_CARRIER || shipType == ShipType::CARRIER || shipType == ShipType::BATTLESHIP || shipType == ShipType::CRUISER){
		m_tileSpan = glm::vec2(2, 1);
	}
	else m_tileSpan = glm::vec2(1, 1);
	m_bounds.initialize(grid->getScreenPos(m_position).x + 2.0f, grid->getScreenPos(m_position).y + 2.0f, m_tileSpan.x * grid->getTileDims().x - 4.0f, m_tileSpan.y * grid->getTileDims().y - 4.0f, true);
}
