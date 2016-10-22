#pragma once

#include <Sakura/Window.h>
#include <Sakura/DebugRenderer.h>

struct Grid
{
public:
	Grid(){ /* Empty */ }
	Grid(glm::ivec2 gridSize, glm::vec2 borderOffset, Sakura::Window* window){ init(gridSize, borderOffset, window); }
	~Grid(){ /* Empty */ }

	void init(glm::ivec2 gridSize, glm::vec2 borderOffset, Sakura::Window* window){
		m_gridDims = gridSize;
		m_gridPos = borderOffset;
		m_tileDims = glm::vec2((window->getScreenWidth() - borderOffset.x * 2) / (float)gridSize.x,
			(window->getScreenHeight() - borderOffset.y * 2) / (float)gridSize.y);
	}

	void resize(glm::ivec2 newSize, Sakura::Window* window){
		init(newSize, m_gridPos, window);
	}

	const glm::ivec2 getDims() const{ return glm::ivec2((float)m_gridDims.x, (float)m_gridDims.y); }
	const glm::vec2 getScreenPos(const glm::ivec2& tilePos) const{ return glm::vec2(((float)tilePos.x * m_tileDims.x) + m_gridPos.x, 
																((float)tilePos.y * m_tileDims.y) + m_gridPos.y); }
	const glm::ivec2 getGridPos(const glm::vec2& absolutePos) const{ return glm::ivec2((absolutePos.x - m_gridPos.x) / m_tileDims.x,
																(absolutePos.y - m_gridPos.y) / m_tileDims.y); }
	const glm::vec2& getTileDims() const{ return m_tileDims; }

	void drawDebug(Sakura::DebugRenderer& debugRenderer){
		for (int i = 0; i < m_gridDims.y; ++i){
			for (int j = 0; j < m_gridDims.x; ++j){
				debugRenderer.drawBox(glm::vec4(j*m_tileDims.x + m_gridPos.x, i*m_tileDims.y + m_gridPos.y, m_tileDims), Sakura::ColorRGBA8(0, 255, 255, 255), 0);
			}
		}
	}
private:
	glm::vec2 m_tileDims;
	glm::vec2 m_gridPos;
	glm::ivec2 m_gridDims;
};

