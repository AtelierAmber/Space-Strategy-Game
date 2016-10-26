#include "Interceptor.h"
#include "Fleet.h"
#include "Grid.h"
#include <algorithm>

#define heart_spacing 2

Interceptor::Interceptor(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team,
	ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, bool isVisible){
	init(grid, fleet, resourceManager, team, shipType, position, enemy, 10, 5, 2, 1, 0, 3, 5, DamageEffect(POWERSHORTAGE, 1.0f, 0.25f, 2));
	if (!isVisible){
		cloak();
	}
}

Interceptor::~Interceptor(){

}

int Interceptor::update(float deltaTime, Grid* grid){
	if (!m_hasUpdatedOnce){
		calculateFriendlyEffects();
		if (calculateBadEffects()){
			return -1;
		}
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
	m_position = grid->getGridPosClamped(glm::vec2(m_bounds.x1, m_bounds.y2));
	return 0;
}

void Interceptor::draw(Sakura::SpriteBatch& spriteBatch, Grid* grid, bool hover){
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
	glm::vec2 shipSize = glm::vec2((m_texture.texture.width / m_texture.dims.x) * shipScale * m_tileSpan.x, (m_texture.texture.height / m_texture.dims.y) * shipScale * m_tileSpan.y + 8.0f);
	glm::vec4 destRect = glm::vec4(m_bounds.x1 + ((m_bounds.width / 2.0f) - (shipSize.x / 2.0f)), m_bounds.y2 + ((m_bounds.height / 2.0f) - (shipSize.y / 2.0f)), shipSize.x, shipSize.y);
	if (m_isSelected){
		float scaler = 4 * std::abs(std::sin(m_selectedSin));
		destRect.x -= scaler / 2;
		destRect.y -= scaler / 2;
		destRect.z += scaler;
		destRect.w += scaler;
		m_selectedSin += 0.05f;
	}
	spriteBatch.draw(destRect, uvRect, m_texture.texture.id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
	if (m_shield){
		spriteBatch.draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_shieldTex.id, 0.1f, Sakura::ColorRGBA8(255, 255, 255, 255));
	}
	if (hover && m_shipType != ShipType::COMMANDSHIP){
#define health_scale 2.0f
		destRect = glm::vec4(m_bounds.x1 + ((m_tileSpan.x - 1) * (m_bounds.width / m_tileSpan.x)) - ((std::max(m_hullMax, m_shieldMax) * (m_hearts.texture.width / 3.0f) * health_scale) / 2.0f), m_bounds.y1 + 5.0f, m_hearts.texture.width / 3.0f * health_scale, m_hearts.texture.height * health_scale);
		for (int i = 0; i < m_hullMax; ++i){
			int uv = (i < m_hull) ? 1 : 2;
			destRect.x += (destRect.z + heart_spacing);
			destRect.y += (int)(i / 15);
			spriteBatch.draw(destRect, m_hearts.getUVs(uv), m_hearts.texture.id, 10.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		}
		destRect.x = m_bounds.x1 + ((m_tileSpan.x - 1) * (m_bounds.width / m_tileSpan.x)) - ((std::max(m_hullMax, m_shieldMax) * (m_hearts.texture.width / 3.0f) * health_scale) / 2.0f);
		for (int i = 0; i < m_shield; ++i){
			destRect.x += (destRect.z + heart_spacing);
			destRect.y += (int)(i / 15);
			spriteBatch.draw(destRect, m_hearts.getUVs(0), m_hearts.texture.id, 10.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		}
	}
	if (m_position != m_newPosition && !m_enemy){
		drawTravelTrail(spriteBatch, grid);
	}
	if (m_queuedAttack  && !m_enemy){
		drawAttackTrail(spriteBatch, grid);
	}
}
