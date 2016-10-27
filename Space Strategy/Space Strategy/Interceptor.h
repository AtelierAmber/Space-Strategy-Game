#pragma once
#include "Ship.h"

#define CLOAKCOOLDOWN 2

class Interceptor :
	public Ship
{
public:
	Interceptor(Grid* grid, Fleet* fleet, Sakura::ResourceManager &resourceManager, std::string team,
		ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, bool isVisible);
	~Interceptor();

	void cloak(){ 
		if (m_currentCloakCooldown <= 0){
			m_cloakTimer = m_cloakMaxTimer;
			m_isVisible = false;
		}
	}
	void uncloak(){ 
		m_isVisible = true; 
		m_cloakTimer = 0;
		m_currentCloakCooldown = CLOAKCOOLDOWN;
	}

	int update(float deltaTime, Grid* grid) override;

	void damageOther(Ship* otherShip) override;

	void draw(Sakura::SpriteBatch& spriteBatch, Grid* grid, bool hover) override;

	int callUnique(void* data) override;

private:
	int m_cloakTimer = 0;
	int m_cloakMaxTimer = 3;
	int m_currentCloakCooldown = 0;
	bool m_isVisible;
};

