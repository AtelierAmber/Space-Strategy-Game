#pragma once

#include <Sakura/Window.h>
#include <Sakura/DebugRenderer.h>

#include <algorithm>

struct Grid
{
public:
	Grid() : m_tileDims(0.0f), m_borderOffset(0.0f), m_gridDims(0){ /* Empty */ }
	Grid(glm::ivec2 gridSize, glm::vec4 borderOffset, Sakura::Window* window){ init(gridSize, borderOffset, window); }
	~Grid(){ /* Empty */ }

	void init(glm::ivec2 gridSize, glm::vec4 borderOffset, Sakura::Window* window){
		m_gridDims = gridSize;
		m_borderOffset = borderOffset;
		m_tileDims = glm::vec2((window->getScreenWidth() - (borderOffset.x + borderOffset.z)) / (float)gridSize.x,
			(window->getScreenHeight() - (borderOffset.y + borderOffset.w)) / (float)gridSize.y);
	}

	void resize(glm::ivec2 newSize, Sakura::Window* window){
		init(newSize, m_borderOffset, window);
	}

	const glm::ivec2 getDims() const{ return glm::ivec2((float)m_gridDims.x, (float)m_gridDims.y); }
	const glm::vec2 getScreenPos(const glm::ivec2& tilePos) const{ return glm::vec2(std::round(((float)tilePos.x * m_tileDims.x) + m_borderOffset.x),
																			std::round(((float)tilePos.y * m_tileDims.y) + m_borderOffset.w));
	}
	const glm::ivec2 getGridPos(const glm::vec2& absolutePos) const{
		if (absolutePos.x < (m_borderOffset.x)){
			return glm::ivec2(-1);
		}
		if (absolutePos.y < (m_borderOffset.w)){
			return glm::ivec2(-1);
		}
		if (absolutePos.x - m_borderOffset.x > (m_tileDims.x * m_gridDims.x)){
			return glm::ivec2(-1);
		}
		if (absolutePos.y - m_borderOffset.w > (m_tileDims.y * m_gridDims.y)){
			return glm::ivec2(-1);
		}
		return glm::ivec2((int)((absolutePos.x - this->m_borderOffset.x) / this->m_tileDims.x),
			(int)((absolutePos.y - this->m_borderOffset.w) / this->m_tileDims.y));
	}

	const glm::ivec2 getGridPosClamped(const glm::vec2& absolutePos) const{
		glm::ivec2 returnVec = glm::ivec2((int)((absolutePos.x - this->m_borderOffset.x) / this->m_tileDims.x),
			(int)((absolutePos.y - this->m_borderOffset.w) / this->m_tileDims.y));;
		if (returnVec.x < 0){
			returnVec.x = 0;
		}
		if (returnVec.y < 0){
			returnVec.y = 0;
		}
		if (returnVec.x > m_gridDims.x){
			returnVec.x = m_gridDims.x;
		}
		if (returnVec.y > m_gridDims.y){
			returnVec.y = m_gridDims.y;
		}
		return returnVec;
	}

	const glm::vec2& getTileDims() const{ return m_tileDims; }

	void drawDebug(Sakura::DebugRenderer& debugRenderer){
		for (int i = 0; i < m_gridDims.y; ++i){
			for (int j = 0; j < m_gridDims.x; ++j){
				debugRenderer.drawBox(glm::vec4(j*m_tileDims.x + m_borderOffset.x, i*m_tileDims.y + m_borderOffset.w, m_tileDims), Sakura::ColorRGBA8(0, 255, 255, 255), 0);
			}
		}
	}
private:
	glm::vec2 m_tileDims;
	glm::vec4 m_borderOffset;
	glm::ivec2 m_gridDims;
};

