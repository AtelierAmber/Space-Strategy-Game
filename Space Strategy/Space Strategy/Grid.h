#pragma once
#include <vector>

#include <Sakura/Window.h>
#include <Sakura/DebugRenderer.h>

class Ship;

class Grid
{
public:
	Grid();
	Grid(glm::ivec2 gridSize, glm::vec2 borderOffset, Sakura::Window* window);
	~Grid();

	void init(glm::ivec2 gridSize, glm::vec2 borderOffset, Sakura::Window* window);
	void destroy();

	void resize(glm::ivec2 newSize, Sakura::Window* window, bool keepShips = true);

	int addShip(glm::ivec2 position, Ship* ship);
	int removeShip(glm::ivec2 position);
	int removeShip(Ship* ship);
	Ship* getShip(glm::ivec2 position);

	const glm::ivec2 getDims(){ return glm::ivec2((float)m_grid[0].size(), (float)m_grid.size()); }
	const glm::vec2 getScreenPos(const glm::ivec2& tilePos){ return glm::vec2((tilePos.x * m_tileDims.x) + m_gridPos.x, 
																(tilePos.y * m_tileDims.y) + m_gridPos.y); }
	const glm::ivec2 getGridPos(const glm::vec2& absolutePos){ return glm::ivec2((absolutePos.x - m_gridPos.x) / m_tileDims.x,
																(absolutePos.y - m_gridPos.y) / m_tileDims.y); }
	const glm::vec2& getTileDims(){ return m_tileDims; }

	void drawDebug(Sakura::DebugRenderer &debugRenderer);
private:
	std::vector<std::vector<Ship*>> m_grid;
	glm::vec2 m_tileDims;
	glm::vec2 m_gridPos;
};

