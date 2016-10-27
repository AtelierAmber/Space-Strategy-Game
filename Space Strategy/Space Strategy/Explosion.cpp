#include "Explosion.h"

void Explosion::init(glm::vec2 position, Sakura::ResourceManager& resourceManager){
	m_position = position;
	m_sheet = resourceManager.getTileSheet("Assets/Sprites/Ships/explosions.png", glm::ivec2(6, 1), MIPMAP | PIXELATED | EDGE_CLAMP);
	m_index = 0.0f;
}

/* Return 1 when finished */
int Explosion::draw(Sakura::SpriteBatch& spriteBatch){
	m_index += anim_speed;
	if (m_index >= 6.0f){
		return 1;
	}
	spriteBatch.draw(glm::vec4(m_position, m_sheet.texture.width / 6, m_sheet.texture.height), m_sheet.getUVs((int)m_index), m_sheet.texture.id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
	return 0;
}
