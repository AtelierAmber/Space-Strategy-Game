#pragma once
#include <vector>

#include <Sakura/Window.h>
#include <Sakura/DebugRenderer.h>
#include "Ship.h"

#include <memory>

class Grid
{
public:
	Grid();
	Grid(glm::ivec2 gridSize, glm::vec2 borderOffset, Sakura::Window* window);
	~Grid();

	void init(glm::ivec2 gridSize, glm::vec2 borderOffset, Sakura::Window* window);
	void destroy();

	void update(float deltaTime, bool playerTurn);

	void resize(glm::ivec2 newSize, Sakura::Window* window, bool keepShips = true);

	int addShip(Sakura::ResourceManager &resourceManager, 
		std::string team, ShipType shipType, glm::ivec2 position /* Position on GRID */, 
		bool enemy, float speed, int shield, int hull, int shieldDamage, int hullDamage, 
		int damageEffectStrength, DamageEffect damageEffect /*= NORMAL*/);
	int destroyShip(const unsigned int& shipID);
	Ship* getShip(glm::ivec2 position);
	int setTile(glm::ivec2 position, Ship* ship);
	
	const std::vector<std::unique_ptr<Ship>>& getShips(){ return m_ships; }

	const glm::ivec2 getDims(){ return glm::ivec2((float)m_grid[0].size(), (float)m_grid.size()); }
	const glm::vec2 getScreenPos(const glm::ivec2& tilePos){ return glm::vec2((tilePos.x * m_tileDims.x) + m_gridPos.x, 
																(tilePos.y * m_tileDims.y) + m_gridPos.y); }
	const glm::ivec2 getGridPos(const glm::vec2& absolutePos){ return glm::ivec2((absolutePos.x - m_gridPos.x) / m_tileDims.x,
																(absolutePos.y - m_gridPos.y) / m_tileDims.y); }
	const glm::vec2& getTileDims(){ return m_tileDims; }

	void drawDebug(Sakura::DebugRenderer &debugRenderer);
	void drawShips(Sakura::SpriteBatch& spriteBatch);
private:
	std::vector<std::vector<Ship*>> m_grid;
	std::vector<std::unique_ptr<Ship>> m_ships;
	glm::vec2 m_tileDims;
	glm::vec2 m_gridPos;
};

