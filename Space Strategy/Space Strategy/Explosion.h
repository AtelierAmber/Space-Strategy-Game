#pragma once

#include <Sakura/SpriteBatch.h>
#include <Sakura/ResourceManager.h>

#define anim_speed 0.1f

class Explosion
{
public:
	Explosion() {/* Empty */}
	Explosion(glm::vec2 position, Sakura::ResourceManager& resourceManager){ init(position, resourceManager); }
	~Explosion() { /* Empty */ }

	void init(glm::vec2 position, Sakura::ResourceManager& resourceManager);

	int draw(Sakura::SpriteBatch& spriteBatch);

	Sakura::TileSheet& getTexture(){ return m_sheet; }

private:
	glm::vec2 m_position;
	Sakura::TileSheet m_sheet;
	float m_index;
};

