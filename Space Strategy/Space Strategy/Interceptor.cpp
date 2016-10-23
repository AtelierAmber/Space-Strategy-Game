#include "Interceptor.h"
#include "Fleet.h"
#include "Grid.h"
#include <algorithm>

#define heart_spacing 2

Interceptor::Interceptor(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team,
	ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, bool isVisible){
	init(grid, fleet, resourceManager, team, shipType, position, enemy, 10, 5, 2, 1, 0, 3, 5, DamageEffect(POWERSHORTAGE, 1.0f, 0.05f, 2));
	if (!isVisible){
		cloak();
	}
}

Interceptor::~Interceptor(){

}

bool Interceptor::update(float deltaTime, Grid* grid){
	if (!m_hasUpdatedOnce){
		calculateFriendlyEffects();
		calculateBadEffects();
		if (m_currentCloakCooldown > 0){
			--m_currentCloakCooldown;
		}
		if (m_cloakTimer > 0){
			--m_cloakTimer;
		}
		else {
			uncloak();
		}
		m_hasUpdatedOnce = true;
	}
	if (m_position != m_newPosition){
		glm::vec2 distToNew = glm::ivec2(m_position.x - m_newPosition.x, m_position.y - m_newPosition.y);
		m_position = m_newPosition;
		distToNew = grid->getScreenPos(distToNew);
		m_bounds.move(-distToNew.x, -distToNew.y);
		return (m_position != m_newPosition);
	}
	//TODO
	return true;
}

void Interceptor::draw(Sakura::SpriteBatch& spriteBatch, bool hover){
	glm::vec4 uvRect = m_texture.getUVs(0);
	if (m_enemy){
		if (!m_isVisible){
			return;
		}
		uvRect = m_texture.getUVs(0);
		uvRect.x += 1.0f / m_texture.dims.x;
		uvRect.z *= -1;
	}
	float shipScale = std::min(m_bounds.width / m_tileSpan.x / (m_texture.texture.width / m_texture.dims.x), m_bounds.height / m_tileSpan.x / (m_texture.texture.height / m_texture.dims.y));
	glm::vec2 shipSize = glm::vec2((m_texture.texture.width / m_texture.dims.x) * shipScale * m_tileSpan.x, (m_texture.texture.height / m_texture.dims.y) * shipScale * m_tileSpan.y);
	glm::vec4 destRect = glm::vec4(m_bounds.x1, m_bounds.y2 + ((m_bounds.height / 2.0f) - (shipSize.y / 2.0f)), shipSize.x, shipSize.y);
	if (m_isSelected){
		float scaler = 4 * std::abs(std::sin(m_selectedSin));
		destRect.x -= scaler / 2;
		destRect.y -= scaler / 2;
		destRect.z += scaler;
		destRect.w += scaler;
		m_selectedSin += 0.05f;
	}
	int shipAlpha = (!m_isVisible) ? 50 : 255;
	spriteBatch.draw(destRect, uvRect, m_texture.texture.id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, shipAlpha));
	if (hover){
		destRect = glm::vec4(m_bounds.x1, m_bounds.y1 + 5, m_hearts.texture.width / 3, m_hearts.texture.height);
		spriteBatch.draw(glm::vec4(destRect.x, destRect.y, destRect.z + (m_hullMax*m_heartContainer.width), destRect.w), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_heartContainer.id, 9.0f, Sakura::ColorRGBA8(255, 255, 255, 255), 0);
		for (int i = 0; i < m_hullMax; ++i){
			destRect.x = m_bounds.x1 + i * (destRect.z + heart_spacing);
			spriteBatch.draw(destRect, m_hearts.getUVs(2), m_hearts.texture.id, 10.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		}
		for (int i = 0; i < m_hull; ++i){
			destRect.x = m_bounds.x1 + i * (destRect.z + heart_spacing);
			spriteBatch.draw(destRect, m_hearts.getUVs(1), m_hearts.texture.id, 10.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		}
		for (int i = 0; i < m_shield; ++i){
			destRect.x = m_bounds.x1 + i * (destRect.z + heart_spacing);
			spriteBatch.draw(destRect, m_hearts.getUVs(0), m_hearts.texture.id, 10.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		}
	}
}
