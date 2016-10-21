	#pragma once

#include <string>
#include <Sakura/Bounds.h>
#include <Sakura/SpriteBatch.h>
#include <Sakura/ResourceManager.h>

class Grid;

enum DamageEffect{
	NORMAL = 0x1, // Normal damage
	FIRE = 0x2, // Periodically damage hull
	EMP = 0x4, // Disable all weapons and shields for the duration
	POWERSHORTAGE = 0x10, // Disable shield and energy weapon recharge
	SUPERCHARGE = 0x20, // FRIENDLY: Boosts shields for the duration
	DAMAGEBOOST = 0x40, // FRIENDLY: Boosts all weapon damage for the duration
	REPAIR = 0x80, // FRIENDLY: Periodically repair the hull
};

typedef std::string ShipType;

#define ASSUALT_CARRIER "assault_carrier.png"
#define BATTLESHIP "battleship.png"
#define CARRIER "carrier.png"
#define CORVETTE "corvette.png"
#define CRUISER "cruiser.png"
#define CUTTER "cutter.png"
#define DESTROYER "destoyer.png"
#define FIGHTER "fighter.png"
#define FRIGATE "frigate.png"
#define INTERCEPTOR "interceptor.png"

class Ship
{
public:
	Ship(Grid* grid, Sakura::ResourceManager &resourceManager, std::string team, ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, float speed, int shield, int hull, int shieldDamage, int hullDamage, int damageEffectStrength, DamageEffect damageEffect = NORMAL);
	Ship() { /* Empty */ }

	virtual ~Ship();

	virtual void init(Grid* grid, Sakura::ResourceManager &resourceManager, std::string team, ShipType shipType, glm::ivec2 position /* Position on GRID */, bool enemy, float speed, int shield, int hull, int shieldDamage, int hullDamage, int damageEffectStrength, DamageEffect damageEffect = NORMAL);
	
	virtual void draw(Sakura::SpriteBatch& spriteBatch);

	void Damage(int hullDamage, int shieldDamage, int effectStrength, DamageEffect statusEffect = NORMAL);

	void ApplyEffect(DamageEffect statusEffect, int effectStrength);

	void damageOther(glm::ivec2 damagePosition);

	void update(float deltaTime, bool isTurn);

	void move(glm::ivec2 newPosition){ m_newPosition = newPosition; }
	const glm::ivec2& getPosition(){ return m_position; };
	const glm::ivec2& getTileSpan(){ return m_tileSpan; }
	const bool& isEnemy(){ return m_enemy; }

	const unsigned int& getID() const { return m_id; }
	void setID(unsigned int ID) { m_id = ID; }

	bool hasMoved(){ return m_hasMoved; }
	void resetMove(){ m_hasMoved = false; }
protected:
	// Damage based in integers (hearts)
	int m_shieldDamage = 0;
	int m_hullDamage = 0;
	DamageEffect m_damageEffect = NORMAL;
	int m_damageEffectStrength = 0;
	int m_range;
	float m_speed;

	bool m_enemy = false;
	bool m_hasMoved = false;

	int m_shieldMax= 5;
	int m_hullMax= 5;
	int m_shield = 5;
	int m_hull = 5;
	DamageEffect m_appliedEffect = NORMAL;
	int m_appliedEffectStrength = 0;

	std::string m_team = "Nuetral";
	ShipType m_shipType;
	unsigned int m_id;

	Grid* m_parentGrid;
	glm::ivec2 m_tileSpan;
	glm::ivec2 m_position;
	glm::vec2 m_absolutePosition;
	glm::ivec2 m_newPosition;
	Sakura::Rect m_selectRect;
	Sakura::TileSheet m_texture;
	Sakura::TileSheet m_hearts;
};