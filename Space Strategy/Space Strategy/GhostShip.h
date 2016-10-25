#pragma once
#include "Ship.h"

struct Grid;
class Fleet;

class GhostShip
{
public:
	GhostShip() : m_position(0.0f){ /* Empty */}
	~GhostShip();

	void update(Grid* grid, glm::vec2 mouseCoords);

	void draw(Sakura::SpriteBatch& spriteBatch, Fleet* playerFleet, Fleet* enemyFleet);
	void drawDebug(Sakura::DebugRenderer& debugRenderer);

	bool checkCollision(Fleet* fleet);
	void setShipType(ShipType shipType, Sakura::ResourceManager &resourceManager, const std::string& team, Grid* grid);
	ShipType getShipType(){ return m_shipType; }
	glm::vec2 getPosition(){ return m_position; }
	Sakura::Rect getBounds(){ return m_bounds; }

private:
	std::string m_team = "Nuetral";
	ShipType m_shipType;
	glm::vec2 m_position;
	Sakura::Rect m_bounds;
	glm::ivec2 m_tileSpan;
	Sakura::TileSheet m_texture;
};

