#include "Cutter.h"
#include <algorithm>

Cutter::Cutter(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team,
	ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, bool costsCP){
	init(grid, fleet, resourceManager, team, shipType, position, enemy, 10.0f, 2, 2, 1, 1, 1, 1 * (int)costsCP, DamageEffect());
	m_numShips = 3;
}

Cutter::~Cutter(){

}

int Cutter::increaseShips(){
	if (m_numShips != 5){
		m_numShips += 2;
		m_shield += 2 * m_shieldMax;
		m_hullMax += 2 * m_hullMax;
		return 0;
	}
	else return -1;
}

int Cutter::decreaseShips(){
	if (m_numShips != 1){
		m_numShips -= 2;
		m_shield -= 2 * m_shieldMax;
		m_hullMax -= 2 * m_hullMax;
		return 0;
	}
	else return -1;
}

int Cutter::Damage(int hullDamage, int shieldDamage, DamageEffect statusEffect){
	std::mt19937 gen((unsigned int)time(nullptr));
	std::uniform_real_distribution<float> percent(0.0f, 1.0f);
	if (percent(gen) <= (1.0f/(float)m_numShips)){
		if (statusEffect.effect != FIRE){
			ApplyEffect(statusEffect);
		}
		m_shield -= shieldDamage;
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
	}
	return 0;
}

#define heart_spacing 2
void Cutter::draw(Sakura::SpriteBatch& spriteBatch, bool hover){
	glm::vec4 uvRect = m_texture.getUVs(m_numShips/2);
	if (m_enemy){
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
	spriteBatch.draw(destRect, uvRect, m_texture.texture.id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
	if (hover){
#define health_scale 2.0f
		destRect = glm::vec4(m_bounds.x1 + ((m_tileSpan.x - 1) * (m_bounds.width / m_tileSpan.x)) - ((std::max(m_hullMax, m_shieldMax) * (m_hearts.texture.width / 3.0f) * health_scale) / 2.0f), m_bounds.y1 + 5.0f, m_hearts.texture.width / 3.0f * health_scale, m_hearts.texture.height * health_scale);
		for (int i = 0; i < m_hullMax; ++i){
			int uv = (i < m_hull) ? 1 : 2;
			destRect.x += (destRect.z + heart_spacing);
			spriteBatch.draw(destRect, m_hearts.getUVs(uv), m_hearts.texture.id, 10.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		}
		destRect.x = m_bounds.x1 + ((m_tileSpan.x - 1) * (m_bounds.width / m_tileSpan.x)) - ((std::max(m_hullMax, m_shieldMax) * (m_hearts.texture.width / 3.0f) * health_scale) / 2.0f);
		for (int i = 0; i < m_shield; ++i){
			destRect.x += (destRect.z + heart_spacing);
			spriteBatch.draw(destRect, m_hearts.getUVs(0), m_hearts.texture.id, 10.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		}
	}
}
