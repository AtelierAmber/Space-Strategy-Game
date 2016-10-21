#include "Grid.h"
#include "Ship.h"

Grid::Grid(){

}
Grid::Grid(glm::ivec2 gridSize, glm::vec2 borderOffset, Sakura::Window* window){
	init(gridSize, borderOffset, window);
}

Grid::~Grid(){
	destroy();
}

void Grid::init(glm::ivec2 gridSize, glm::vec2 borderOffset, Sakura::Window* window){
	m_gridPos = borderOffset;
	m_grid.resize(gridSize.y);
	for (int i = 0; i < gridSize.y; ++i){
		m_grid[i].resize(gridSize.x, nullptr);
	}
	m_tileDims = glm::vec2((window->getScreenWidth() - borderOffset.x*2) / gridSize.x, 
						  (window->getScreenHeight() - borderOffset.y*2) / gridSize.y);
}

void Grid::destroy(){
	for (std::size_t i = 0; i < m_grid.size(); ++i){
		m_grid[i].clear();
	}
	m_grid.clear();
}

void Grid::resize(glm::ivec2 newSize, Sakura::Window* window, bool keepShips){
	m_grid.resize(newSize.y);
	for (int i = 0; i < newSize.y; ++i){
		if (!keepShips){
			m_grid[i].clear();
		}
		m_grid[i].resize(newSize.x, nullptr);
	}
	m_tileDims = glm::vec2((window->getScreenWidth() - m_gridPos.x * 2) / newSize.x,
						  (window->getScreenHeight() - m_gridPos.y * 2) / newSize.y);
}

int Grid::addShip(glm::ivec2 position, Ship* ship){
	if (position.y > (int)m_grid.size()-1){
		return -1;
	}
	if (position.x > (int)m_grid[position.y].size()-1){
		return -1;
	}
	m_grid[position.y][position.x] = ship;
	return 0;
}

int Grid::removeShip(glm::ivec2 position){
	if (position.y > (int)m_grid.size()-1){
		return -1;
	}
	if (position.x > (int)m_grid[position.y].size()-1){
		return -1;
	}
	m_grid[position.y][position.x] = nullptr;
	return 0;
}

int Grid::removeShip(Ship* ship){
	return removeShip(ship->getPosition());
}

Ship* Grid::getShip(glm::ivec2 position){
	if (position.y > (int)m_grid.size()-1){
		return nullptr;
	}
	if (position.x > (int)m_grid[position.y].size()-1){
		return nullptr;
	}
	return m_grid[position.y][position.x];
}

void Grid::drawDebug(Sakura::DebugRenderer &debugRenderer){
	for (std::size_t i = 0; i < m_grid.size(); ++i){
		for (std::size_t j = 0; j < m_grid[i].size(); ++j){
			glm::vec4 destRect = glm::vec4(getScreenPos(glm::ivec2(j, i)), m_tileDims);
			debugRenderer.drawBox(destRect, Sakura::ColorRGBA8(255,255,255,255), 0);
		}
	}
}
